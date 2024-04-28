#include "framework.h"
#include <iostream>
#include <filesystem>
#include "DOOMSona.hpp"
#include "DoomD3DHook.hpp"
#include "DoomAPIIntegration.hpp"

#include "includes/injector/injector.hpp"
#include "includes/assembly64.hpp"

namespace DOOMSona
{
	constexpr uintptr_t pGetScriptArg = 0x1416E83F0;
	constexpr uintptr_t pScriptContext = 0x142ADCC28;
	uint32_t personaCurrBGM;

	namespace DOOM
	{
		static void PerformAtDoomExit()
		{
			DoomD3DHook::SetFramebufferEnabled(false);
		}

		static bool Launch()
		{
			if (!DoomAPI::modHandle)
			{
				if (!DoomAPI::Init("ChocoDoom.dll"))
					return false;
			}

			DoomAPI::SetModPath(".");
			DoomD3DHook::SetFramebufferEnabled(true);

			if (!DoomAPI::LaunchDoom())
				return false;

			DoomAPI::DoomRegisterAtExit(PerformAtDoomExit, true);

			return true;
		}
	}

	static bool separate_console(void)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
			return false;

		return true;
	}

	static void OpenConsole()
	{
		if (!separate_console())
		{
			AllocConsole();
			AttachConsole(ATTACH_PARENT_PROCESS);

			freopen("CON", "wb", stdout);
			freopen("CON", "wb", stderr);
		}
	}

	// Persona hooks
	static uint32_t _stdcall GetScriptArg(int num)
	{
		return reinterpret_cast<uint32_t(__thiscall*)(int)>(pGetScriptArg)(num);
	}

	static uint32_t _stdcall CheckDisableSharePlayHook()
	{
		uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);
		uint64_t arg0 = GetScriptArg(0);

		bool bDoomLaunchResult = false;

		if (!DoomAPI::bIsDoomRunning())
			bDoomLaunchResult = DOOM::Launch();

		// return value
		*reinterpret_cast<uint64_t*>(context + 0x1D8) = bDoomLaunchResult;
		*reinterpret_cast<uint8_t*>(context + 0x5F) = 0;


		return 1;
	}

	// multi-tool function - get stats and crap from game by input parameter
	static uint32_t _stdcall TBL365ValueMDHook()
	{
		uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);

		uint32_t arg2 = GetScriptArg(2);
		uint32_t arg1 = GetScriptArg(1);
		uint32_t arg0 = GetScriptArg(0);

		uint64_t retval = 0;

		switch (arg0)
		{
		case DPSP_SECRETS:
			break;
		case DPSP_KILLS:
			break;
		case DPSP_CURRBGM:
			retval = personaCurrBGM;
			break;
		case DPSP_ISGAMERUNNING:
			retval = DoomAPI::bIsDoomRunning();
			break;
		default:
			break;
		}

		// return value
		*reinterpret_cast<uint64_t*>(context + 0x1D8) = retval;
		*reinterpret_cast<uint8_t*>(context + 0x5F) = 0;

		return 1;
	}

	void Init()
	{
		OpenConsole();

		DoomD3DHook::Init();

		struct hkGameLoop
		{
			void operator()(injector::reg_pack& regs)
			{
				if (DoomAPI::bIsDoomRunning())
				{
					regs.rax = 0;
					DoomAPI::DoomMainLoopFunc();
				}
				else
				{
					if (DoomAPI::modHandle)
						DoomAPI::Deinit();
					regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
				}
			}
		}; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);

		struct hkCurrBGM
		{
			void operator()(injector::reg_pack& regs)
			{
				personaCurrBGM = regs.rbx;
				regs.rax = 0x141AAA670;
			}
		}; injector::MakeInline<hkCurrBGM>(0x15590AB38, 0x15590AB3F);


		injector::MakeAbsJMP(0x00000001416E1D20, CheckDisableSharePlayHook);
		injector::MakeAbsJMP(0x00000001416DDFE0, TBL365ValueMDHook);
	}
}