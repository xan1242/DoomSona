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

#include "pch.h"
#include "FrameLimiter.hpp"

#include "includes/injector/injector.hpp"
//#include "includes/injector/assembly.hpp"
#include "includes/assembly64.hpp"

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
//#include "Chocolate DOOM/re_main.h"
}

#include "DoomD3DHook.h"
#include <thread>

const char* modPath;

bool doomed = false;			// flag indicating if DOOM has loaded
bool doomedonce = false;

FrameLimiter::FPSLimitMode mFPSLimitMode;

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

void DoomExited()
{
	doomed = false;
	DoomD3DHook::SetFramebufferEnabled(false);
}

void LaunchDoom()
{
	if (!doomedonce)
		doomed = D_DoomMain();
	else
		doomed = true;
	if (doomed)
	{
		doomedonce = doomed;
		DoomD3DHook::SetFramebufferEnabled(true);
		DoomMode_Init();
		I_AtExit(&DoomExited, false);
	}
}

uint32_t _stdcall AIDebugPrintHook()
{
	LaunchDoom();

	return 1;
}

#pragma runtime_checks( "", restore )

void DoomThread()
{
	while (1)
	{
		DoomMode_Main();

		if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_REALTIME)
			while (!FrameLimiter::Sync_RT());
		else if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
			while (!FrameLimiter::Sync_SLP());
	}
}

void LaunchDoomThread()
{
	std::thread doomThread(DoomThread);
	doomThread.detach();
}

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

extern "C" __declspec(dllexport) void TheInitFunc()
{
	//Pattern::Detector* gPatternDetector = new Pattern::Detector();

	//modPath = path;
	modPath = ".";

	//mFPSLimitMode = FrameLimiter::FPSLimitMode::FPS_ACCURATE;
	//FrameLimiter::Init(mFPSLimitMode, 60.0);

	// input block test
	//uintptr_t loc_140E43469 = ConstPatternInlineGetFirst(gPatternDetector, "E9 ? ? ? ? 0F 28 C7 0F 28 7C 24 20", 0);
	//printf("loc_140E43469: 0x%llX\n", loc_140E43469);
	////injector::MakeNOP(loc_140E43469, 5);
	//uintptr_t sub_1418D9FF0 = static_cast<uintptr_t>(injector::GetBranchDestination(loc_140E43469));
	//printf("sub_1418D9FF0: 0x%llX\n", sub_1418D9FF0);
	//injector::MakeRET(sub_1418D9FF0);
	//injector::MakeRET(0x1418D9AA0);
	//injector::MakeJMP(0x1414F6FC4, 0x1414F7018);
	//injector::MakeRET(0x14024FFA0);
	//injector::MakeCALL(0x14022B70E, sub_14024FFA0_hook);
	//injector::MakeCALL(0x1401D67FB, sub_14024FFA0_hook);

	//injector::MakeNOP(0x1401D680C, 2);
	//injector::MakeNOP(0x1401D680E, 7);

	
	OpenConsole();
	//LaunchDoom();

	DoomD3DHook::Init();

	struct hkGameLoop
	{
		void operator()(injector::reg_pack& regs)
		{
			if (doomed)
			{
				regs.rax = 0;
				DoomMode_Main();
			}
			else
				regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
		}
	}; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);

	injector::MakeAbsJMP(0x0000000140825010, AIDebugPrintHook);
	//injector::MakeJMP(0x0000000140825010, AIDebugPrintHook);
	//injector::MakeJMP(0x0000000140825000, AIDebugPrintHook);

	//struct hkAIDebugPrint
	//{
	//	void operator()(injector::reg_pack& regs)
	//	{
	//		if (doomed)
	//		{
	//			regs.rax = 0;
	//			DoomMode_Main();
	//		}
	//		else
	//			regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
	//	}
	//}; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);

	//delete gPatternDetector;

	//LaunchDoomThread();
}

extern "C" __declspec(dllexport) void InitializeASI()
{
	return TheInitFunc();
}
