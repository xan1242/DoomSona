#include "framework.h"
#include <iostream>
#include <filesystem>
#include "DOOMSona.hpp"
#include "DoomD3DHook.hpp"
#include "DoomAPIIntegration.hpp"

#include "includes/injector/injector.hpp"
#include "includes/assembly64.hpp"

#include "dxhook/src/MemoryUtils.h"

namespace DOOMSona
{
	constexpr uintptr_t pGetScriptArg = 0x1416E83F0;
	constexpr uintptr_t pScriptContext = 0x142ADCC28;
	uint32_t personaCurrBGM;

	bool bUndoCBT;

	constexpr std::string StringParamKeyword = "CACODEMON ";
	std::string currStringParam;

	std::string currChocoDoomArgs = "";

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

			printf("DOOMSona: Launching DOOM with args: %s\n", currChocoDoomArgs.c_str());

			if (!DoomAPI::LaunchDoom(currChocoDoomArgs.c_str()))
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

	static char* GetScriptArgStr(int num)
	{
		uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);
		uint32_t argCount = *reinterpret_cast<uint32_t*>(context + 0x2C);

		uint8_t argType = *reinterpret_cast<uint8_t*>(context + 0x2F + (argCount - num));

		if (argType != 5)
			return nullptr;


		return *reinterpret_cast<char**>(context + 0x58 + (sizeof(uint64_t) * (argCount - num)));
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
		case DPSP_APPLYDOOMARGS:
			currChocoDoomArgs = currStringParam;
			break;
		case DPSP_FILESIZE:
			retval = std::filesystem::file_size(currStringParam);
			break;
		case DPSP_FILEEXISTS:
			retval = std::filesystem::exists(currStringParam);
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

	static bool bIncludesStrParamKeyword(const char* str)
	{
		if (!str)
			return false;

		std::string inString = str;
		if (inString.empty())
			return false;

		if (inString.length() < StringParamKeyword.length())
			return false; // The string is shorter than the keyword

		std::transform(inString.begin(), inString.end(), inString.begin(), ::toupper);

		for (size_t i = 0; i < StringParamKeyword.length(); ++i)
		{
			if (inString[i] != StringParamKeyword[i])
			{
				return false; // Characters don't match
			}
		}

		return true;
	}

	static void CopyStringParam(const char* str)
	{
		std::string inString = str;
		currStringParam = inString.substr(StringParamKeyword.length());
	}

	static uint32_t _stdcall DbgPutsHook()
	{
		//uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);

		//MemoryUtils::PrintBytesAtAddress(context, 0x200);

		char* in_str = GetScriptArgStr(0);

		if (in_str)
		{
			if (bIncludesStrParamKeyword(in_str))
			{
				CopyStringParam(in_str);
				//printf("CACODEMON is: %s\n", currStringParam.c_str());
			}
			else
				printf("DBG_PUTS: %s\n", in_str);
		}

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
				if (!bUndoCBT)
				{
					bUndoCBT = true;
					injector::WriteMemory<uintptr_t>(0x0000000141AA7270, 0x1416DD550, true);
				}

				if (DoomAPI::bIsDoomRunning() && !DoomAPI::bHasDoomErrored())
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
		injector::MakeAbsJMP(0x00000001416DD550, DbgPutsHook);
		// increase the cmmBookTable limit to 127 (this is as much as we can do without hooking code since it's a 'cmp esi, 7Fh' here)
		injector::WriteMemory<uint8_t>(0x0000000140C8993B + 2, 0x7F, true); 
	}
}