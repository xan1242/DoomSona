//
// Copyright(C) 2020 TheStoneBanana
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Mania mod entrypoint
//

//#define USE_SEPARATE_THREAD

#include "pch.h"
#include "MainExports.hpp"

//#include "FrameLimiter.hpp"

//#include "includes/injector/injector.hpp"
//#include "includes/injector/assembly.hpp"
//#include "includes/assembly64.hpp"

//#define PATTERNS_WIN32
//#define PATTERNS_CONSTEXPR
//#include "includes/patterns.hpp"

//FunctionPointer(SonicMania::Controller*, Engine_ProcessInput, (), 0x1CDB40);
//FunctionPointer(int, Engine_ProcessSceneTimer, (), 0x1F2600);
//
//FunctionPointer(void, Engine_StartGameObjects, (), 0x1C6490);

extern "C" {
#include "Chocolate DOOM/d_main.h"
#include "Chocolate DOOM/z_zone.h"
#include "Chocolate DOOM/i_system.h"
//#include "Chocolate DOOM/w_wad.h"
//#include "Chocolate DOOM/re_main.h"
}

//#include "DoomD3DHook.h"
//#include <thread>

//#ifdef _DEBUG

//#include "dxhook/src/MemoryUtils.h"

//#endif

const char* modPath;

bool doomed = false;			// flag indicating if DOOM has loaded
bool doomedonce = false;

// PERSONA STUFF
//uint16_t personaCurrBGM;
//FrameLimiter::FPSLimitMode mFPSLimitMode;

// ----------------------------------------------------------------------------
// DOOM's engine state
// ----------------------------------------------------------------------------

void DoomMode_Main(void) 
{
	D_DoomLoop();
}

// ----------------------------------------------------------------------------
// Preparing to meet our doom
// ----------------------------------------------------------------------------
static void DoomMode_Init(void) 
{
	D_DoomInit_Lite();
}

#pragma runtime_checks( "", off )

constexpr uintptr_t pScriptContext = 0x142ADCC28;

//#pragma pack(push, 1)
//struct tScriptContext
//{
//	uint8_t pad[0x2C];			// 000-02C
//	uint16_t m_nNumArgs;		// 02C-02E
//	uint8_t pad2;				// 02E-02F
//	uint8_t m_nArgTypes[32];	// 02F-04F
//	uint8_t pad3[0x11];			// 04F-058
//	uint64_t m_nArguments[32];	// 058-158
//	uint8_t pad4[0x7E];			// 158-1D8
//	uint32_t m_nReturn = 0;		// 1D8-1DC
//	uint8_t pad5[0x48];			// 1DC-224
//	uint32_t m_nWaitingFlag;	// 224-228
//	uint8_t pad6[0x200];		// 228-428 massive pad since im not sure how big this is
//};
//#pragma pack(pop)

bool bIsDoomRunning()
{
	return doomed;
}

void DoomExited()
{
	doomed = false;
}

// #TODO: expand for wad selection
bool LaunchDoom()
{
	if (!doomedonce)
		doomed = D_DoomMain();
	else
	{
		doomed = true;
	}
	if (doomed)
	{
		doomedonce = doomed;
		DoomMode_Init();
		I_AtExit(&DoomExited, false);
	}

	return doomed;
}

void DoomMainLoopFunc()
{
	return DoomMode_Main();
}

void SetModPath(const char* path)
{
	modPath = path;
}

void DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error)
{
	I_AtExit(func, run_if_error);
	return;
}

//enum DoomPersonaScriptParams
//{
//	DPSP_UNK = -1,
//	DPSP_ISGAMERUNNING,
//	DPSP_CURRBGM,
//	DPSP_KILLS,
//	DPSP_SECRETS,
//	
//	DPSP_COUNT
//};

//const char* DoomPersonaScriptParamsNames[] =
//{
//	"DPSP_ISGAMERUNNING",
//	"DPSP_CURRBGM",
//	"DPSP_KILLS",
//	"DPSP_SECRETS",
//};

//tScriptContext* GetPersonaContext()
//{
//	return *reinterpret_cast<tScriptContext**>(pScriptContext);
//}

//uint32_t _stdcall GetScriptArg(int num)
//{
//	return reinterpret_cast<uint32_t(__thiscall*)(int)>(0x1416E83F0)(num);
//}
//
//// multi-tool function - get stats and crap from game by input parameter
//uint32_t _stdcall TBL365ValueMDHook()
//{
//	uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);
//
//
//
//	//uint64_t arg2 = *reinterpret_cast<uint64_t*>(context + 0x88);
//	//uint64_t arg1 = *reinterpret_cast<uint64_t*>(context + 0x88 + sizeof(uint64_t));
//	//uint64_t arg0 = *reinterpret_cast<uint64_t*>(context + 0x88 + sizeof(uint64_t) +sizeof(uint64_t));
//
//	uint64_t arg2 = GetScriptArg(2);
//	uint64_t arg1 = GetScriptArg(1);
//	uint64_t arg0 = GetScriptArg(0);
//
//	uint64_t retval = 0;
//
//	//printf("TBL365ValueMDHook: args: %lld %lld %lld\n", arg0, arg1, arg2);
//
//	//MemoryUtils::PrintBytesAtAddress(context, 0x200);
//
//
//	switch (arg0)
//	{
//	case DPSP_SECRETS:
//		break;
//	case DPSP_KILLS:
//		break;
//	case DPSP_CURRBGM:
//		retval = personaCurrBGM;
//		break;
//	case DPSP_ISGAMERUNNING:
//		retval = doomed;
//		break;
//	default:
//		break;
//	}
//
//	//printf("TBL365ValueMDHook: returning: %lld\n", retval);
//
//	// return value
//	*reinterpret_cast<uint64_t*>(context + 0x1D8) = retval;
//	*reinterpret_cast<uint8_t*>(context + 0x5F) = 0;
//
//	return 1;
//}
//
//uint32_t _stdcall CheckDisableSharePlayHook()
//{
//	//tScriptContext* context = GetPersonaContext();
//
//	uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);
//
//	uint64_t arg0 = *reinterpret_cast<uint64_t*>(context + 0x60);
//
//	if (!doomed)
//		LaunchDoom();
//
//	// return value
//	*reinterpret_cast<uint64_t*>(context + 0x1D8) = doomed;
//	*reinterpret_cast<uint8_t*>(context + 0x5F) = 0;
//
//
//	return 1;
//}

//uint32_t _stdcall BGMFuncHook()
//{
//	int argVal = reinterpret_cast<int(__thiscall*)(int)>(0x1416E83F0)(0);
//
//	printf("BGM: %d\n", argVal);
//
//	reinterpret_cast<void(__thiscall*)(int)>(0x14178AA40)(argVal);
//
//	return 1;
//}

#pragma runtime_checks( "", restore )

//void DoomThread()
//{
//	while (1)
//	{
//		if (doomed)
//		{
//			DoomMode_Main();
//
//			if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_REALTIME)
//				while (!FrameLimiter::Sync_RT());
//			else if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
//				while (!FrameLimiter::Sync_SLP());
//		}
//
//		if ((GetAsyncKeyState(VK_F4) >> 15) & 1)
//		{
//			if (!doomed)
//				LaunchDoom();
//		}
//
//	}
//}
//
//void LaunchDoomThread()
//{
//	std::thread doomThread(DoomThread);
//	doomThread.detach();
//}

static bool separate_console(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return false;

	return true;
}

void OpenConsole()
{
	if (!separate_console())
	{
		AllocConsole();
		AttachConsole(ATTACH_PARENT_PROCESS);

		freopen("CON", "wb", stdout);
		freopen("CON", "wb", stderr);
	}
}

//extern "C" __declspec(dllexport) void TheInitFunc()
//{
//	//Pattern::Detector* gPatternDetector = new Pattern::Detector();
//
//	//modPath = path;
//	modPath = ".";
//
//	//mFPSLimitMode = FrameLimiter::FPSLimitMode::FPS_ACCURATE;
//	//FrameLimiter::Init(mFPSLimitMode, 60.0);
//
//	// input block test
//	//uintptr_t loc_140E43469 = ConstPatternInlineGetFirst(gPatternDetector, "E9 ? ? ? ? 0F 28 C7 0F 28 7C 24 20", 0);
//	//printf("loc_140E43469: 0x%llX\n", loc_140E43469);
//	////injector::MakeNOP(loc_140E43469, 5);
//	//uintptr_t sub_1418D9FF0 = static_cast<uintptr_t>(injector::GetBranchDestination(loc_140E43469));
//	//printf("sub_1418D9FF0: 0x%llX\n", sub_1418D9FF0);
//	//injector::MakeRET(sub_1418D9FF0);
//	//injector::MakeRET(0x1418D9AA0);
//	//injector::MakeJMP(0x1414F6FC4, 0x1414F7018);
//	//injector::MakeRET(0x14024FFA0);
//	//injector::MakeCALL(0x14022B70E, sub_14024FFA0_hook);
//	//injector::MakeCALL(0x1401D67FB, sub_14024FFA0_hook);
//
//	//injector::MakeNOP(0x1401D680C, 2);
//	//injector::MakeNOP(0x1401D680E, 7);
//
//	
//	OpenConsole();
////	DoomD3DHook::Init();
////
////#ifndef USE_SEPARATE_THREAD
////	//LaunchDoom();
////
////
////	struct hkGameLoop
////	{
////		void operator()(injector::reg_pack& regs)
////		{
////			if (doomed)
////			{
////				regs.rax = 0;
////				DoomMode_Main();
////			}
////			else
////				regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
////		}
////	}; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);
////
////	//struct hkCurrBGM
////	//{
////	//	void operator()(injector::reg_pack& regs)
////	//	{
////	//		personaCurrBGM = regs.rax & 0xFFFF;
////	//		printf("Current BGM: %d\n", personaCurrBGM);
////	//		regs.rcx = *reinterpret_cast<uintptr_t*>(0x142ADCE68);
////	//	}
////	//}; injector::MakeInline<hkCurrBGM>(0x00000001558A9EAB, 0x00000001558A9EB2);
////
////	struct hkCurrBGM
////	{
////		void operator()(injector::reg_pack& regs)
////		{
////			personaCurrBGM = regs.rbx;
////			//printf("Current BGM: %d\n", personaCurrBGM);
////			regs.rax = 0x141AAA670;
////		}
////	}; injector::MakeInline<hkCurrBGM>(0x15590AB38, 0x15590AB3F);
////
////	//injector::MakeAbsJMP(0x1416D9E90, BGMFuncHook);
////
////	injector::MakeAbsJMP(0x00000001416E1D20, CheckDisableSharePlayHook);
////	injector::MakeAbsJMP(0x00000001416DDFE0, TBL365ValueMDHook);
////	//injector::MakeAbsJMP(0x0000000140825010, AIDebugPrintHook);
////	//injector::MakeAbsJMP(0x0000000140825000, AIDebugPrintValueHook);
////
////	//struct hkAIDebugPrint
////	//{
////	//	void operator()(injector::reg_pack& regs)
////	//	{
////	//		if (doomed)
////	//		{
////	//			regs.rax = 0;
////	//			DoomMode_Main();
////	//		}
////	//		else
////	//			regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
////	//	}
////	//}; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);
////
////	//delete gPatternDetector;
////
////#else
////	//LaunchDoom();
////	LaunchDoomThread();
////
////#endif // USE_SEPARATE_THREAD
//}

//extern "C" __declspec(dllexport) void InitializeASI()
//{
//	return TheInitFunc();
//}
