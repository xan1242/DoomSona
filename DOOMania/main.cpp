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
#include "Chocolate DOOM/re_main.h"
}

const char* modPath;

int snakeY[424/2];
static unsigned short FramebufferCopy[432 * 240];
int framecounter = 0;

bool doomed = false;			// flag indicating if DOOM has loaded

FrameLimiter::FPSLimitMode mFPSLimitMode;

// ----------------------------------------------------------------------------
// DOOM's engine state
// ----------------------------------------------------------------------------
enum class WIPE_STATES {
	WIPE_DONE,
	WIPE_START,
	WIPE_PROCESS
} wiper;

void DoomMode_Main(void) {
	//Engine_ProcessInput();

	//switch (wiper) {
	//	case (WIPE_STATES::WIPE_START):
	//		memcpy_s(FramebufferCopy, 432 * 240 * sizeof(short), SonicMania::Framebuffer, 432 * 240 * sizeof(short));	// preserve Mania's last rendered framebuffer
	//		wiper = WIPE_STATES::WIPE_PROCESS;																			// start the wipe
	//	case (WIPE_STATES::WIPE_PROCESS):
	//		D_DoomLoop();
	//		for (int x = 0; x < (424 / 2); x++) {
	//			if (snakeY[x] < 240) {
	//				int nakeySnakey = max(0, snakeY[x]);
	//				for (int y = nakeySnakey; y < 240; y++) {
	//					for (int z = 0; z < 2; z++) {
	//						SonicMania::Framebuffer[(y * 432) + (x * 2) + z] = FramebufferCopy[((y - nakeySnakey) * 432) + (x * 2) + z];
	//					}
	//				}
	//			}
	//			snakeY[x] += 5;
	//		}
	//		break;
	//	case (WIPE_STATES::WIPE_DONE):
	//	default:
			D_DoomLoop();
	//		break;
	//}
}

// ----------------------------------------------------------------------------
// Wrapper for the call to Doom's main loop to preserve the stack
// ----------------------------------------------------------------------------
// __declspec(naked) static void DoomMode_Wrapper(void) {
// 	__asm {
// 		call	DoomMode_Main
// 		pop		edi
// 		pop		esi
// 		pop		ebx
// 		retn
// 	}
// }

// ----------------------------------------------------------------------------
// Preparing to meet our doom
// ----------------------------------------------------------------------------
static void DoomMode_Init(void) {
	D_DoomInit_Lite();
	//SonicMania::GameState = (SonicMania::GameStates)0xD;

	// Set up the Mania-to-DOOM screen transition
	//wiper = WIPE_STATES::WIPE_START;
	//snakeY[0] = SonicMania::Rand(-16, 0);
	//for (int i = 1; i < (424 / 2); i++) {
	//	snakeY[i] = snakeY[i - 1] + SonicMania::Rand(-4, 5);
	//	if (snakeY[i] > 0) { snakeY[i] = 0; }
	//}
}

// ----------------------------------------------------------------------------
// Correlates the DOOM MODE button to its diorama
// ----------------------------------------------------------------------------
// const int loc_4620B = baseAddress + 0x04620B;
// __declspec(naked) static void yeahBud(void) {
// 	__asm {
// 		mov		dword ptr [esi + 0x58], 8
// 		jmp		loc_4620B
// 	}
// }

// this has to be defined here because there isn't enough room to squeeze another table entry in the base game
// _declspec(align(0x10)) const int MainMenu_LateUpdate_JmpTable[] = {
// 	baseAddress + 0x0461C5,				// MANIA MODE
// 	baseAddress + 0x0461CE,				// TIME ATTACK
// 	baseAddress + 0x0461D7,				// COMPETITION
// 	baseAddress + 0x0461E0,				// OPTIONS
// 	baseAddress + 0x0461E9,				// EXTRAS
// 	baseAddress + 0x0461F2,				// ENCORE MODE
// 	baseAddress + 0x0461FB,				// BUY DLC
// 	baseAddress + 0x046204,				// EXIT
// 	(int)yeahBud						// DOOM MODE
// };

// enum MENU_BUTTONS {
// 	MENUBUTTON_MANIAMODE,
// 	MENUBUTTON_TIMEATTACK,
// 	MENUBUTTON_COMPETITION,
// 	MENUBUTTON_OPTIONS,
// 	MENUBUTTON_EXTRAS,
// 	MENUBUTTON_ENCOREMODE,
// 	MENUBUTTON_DLC,
// 	MENUBUTTON_EXIT,
// 	MENUBUTTON_DOOMMODE
// };

// ----------------------------------------------------------------------------
// Initializing the main menu's buttons
// ----------------------------------------------------------------------------
// void MainMenu_ButtonInit(void) {
// 	// i know this seems weird to declare locally, but the pointer isn't loaded until the game starts so we can't grab it in the Init function
// 	static int(_cdecl* const RSDK_CheckDLC)(int dlc) = (int(_cdecl*)(int dlc))(*(int*)(baseAddress + 0x0AA7654));
// 
// 	int OBJ_MainMenu_PTR = *(int*)(baseAddress + 0xAC691C);
// 	SonicMania::EntityBase* Child_UIControl = (SonicMania::EntityBase*)*(int*)(OBJ_MainMenu_PTR + 4);
// 	SonicMania::EntityUIButtonz** Childs_UIButton = (SonicMania::EntityUIButtonz**)((int*)(&Child_UIControl->Extra[0xA0]));
// 
// 	int bigButt = 0;
// 
// 	Childs_UIButton[bigButt]->FrameID = MENUBUTTON_MANIAMODE;
// 	Childs_UIButton[bigButt]->Transition = true;
// 	Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 	// If the player has the Mania Plus DLC...
// 	if (RSDK_CheckDLC(0)) {
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_ENCOREMODE;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 		if (doomed) {
// 			Childs_UIButton[bigButt]->FrameID = MENUBUTTON_DOOMMODE;
// 			Childs_UIButton[bigButt]->Transition = false;
// 			Childs_UIButton[bigButt++]->StopMusic = true;
// 		}
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_TIMEATTACK;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_COMPETITION;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_OPTIONS;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = false;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_EXTRAS;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = false;
// 	}
// 	// Otherwise, if the player just has the base game...
// 	else {
// 		if (doomed) {
// 			Childs_UIButton[bigButt]->FrameID = MENUBUTTON_DOOMMODE;
// 			Childs_UIButton[bigButt]->Transition = false;
// 			Childs_UIButton[bigButt++]->StopMusic = true;
// 		}
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_TIMEATTACK;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_COMPETITION;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = true;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_OPTIONS;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = false;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_EXTRAS;
// 		Childs_UIButton[bigButt]->Transition = true;
// 		Childs_UIButton[bigButt++]->StopMusic = false;
// 
// 		Childs_UIButton[bigButt]->FrameID = MENUBUTTON_DLC;
// 		Childs_UIButton[bigButt]->Transition = false;
// 		Childs_UIButton[bigButt++]->StopMusic = false;
// 	}
// 
// 	Childs_UIButton[bigButt]->FrameID = MENUBUTTON_EXIT;
// 	Childs_UIButton[bigButt]->Transition = false;
// 	Childs_UIButton[bigButt++]->StopMusic = false;
// 
// 	if (!doomed) {
// 		Child_UIControl->Extra[0x2A] = Child_UIControl->Extra[0x28] = bigButt;
// 	}
// }

// ----------------------------------------------------------------------------
// Wrapper for the main menu's button init
// ----------------------------------------------------------------------------
// __declspec(naked) static void MainMenu_ButtInit(void) {
// 	__asm {
// 		call	MainMenu_ButtonInit
// 		pop		edi
// 		pop		esi
// 		pop		ebx
// 		mov		esp, ebp
// 		pop		ebp
// 		retn
// 	}
// }

// ----------------------------------------------------------------------------
// Updating DOOM Mode's diorama on the main menu
// ----------------------------------------------------------------------------
// const int moody2 = baseAddress + 0x045FE0;
// float drawDisp = 0.0f;
// SonicMania::EntityAnimationData UFOLamp, TVidle;
// void UIDiorama_Update_DOOMMode(void) {
// 	SonicMania::ActiveEntityInfo* ActiveEnt = (SonicMania::ActiveEntityInfo*)&SonicMania::EntityInfo->CurrentEntity;
// 	// If the animation reload flag has been activated...
// 	if (*(int*)&ActiveEnt->CurrentEntity->Extra[0x10]) {
// 		*(int*)&ActiveEnt->CurrentEntity->Extra[0x10] = false;				// deactivate the reload flag
// 		*(int*)&ActiveEnt->CurrentEntity->Extra[0x20] = 0xFF00;
// 		int OBJ_UIDiorama_PTR = *(int*)(baseAddress + 0xAC6AF4);
// 		short DioramaMappings = *(short*)(OBJ_UIDiorama_PTR + 4);
// 		SonicMania::SetSpriteAnimation(DioramaMappings, 12, (SonicMania::EntityAnimationData*)&ActiveEnt->CurrentEntity->Extra[0x124], true, 0);
// 		SonicMania::SetSpriteAnimation(DioramaMappings, 13, (SonicMania::EntityAnimationData*) &UFOLamp, true, 0);
// 		SonicMania::SetSpriteAnimation(DioramaMappings, 14, (SonicMania::EntityAnimationData*) &TVidle, true, 0);
// 
// 		// secwetly insert some cowors so de awt is pwettier uwu
// 		// (this is in unused palette space)
// 		SonicMania::Palette0[0x90] = SonicMania::ToRGB565(0, 56, 32);	// ufo lamp...
// 		SonicMania::Palette0[0x91] = SonicMania::ToRGB565(0, 88, 64);
// 		SonicMania::Palette0[0x92] = SonicMania::ToRGB565(0, 128, 64);
// 		SonicMania::Palette0[0x93] = SonicMania::ToRGB565(0, 160, 88);
// 		SonicMania::Palette0[0x94] = SonicMania::ToRGB565(8, 192, 128);
// 		SonicMania::Palette0[0x95] = SonicMania::ToRGB565(48, 240, 184);
// 		SonicMania::Palette0[0xDC] = SonicMania::ToRGB565(1, 1, 88);	// clouds...
// 		SonicMania::Palette0[0xDD] = SonicMania::ToRGB565(24, 24, 96);
// 		SonicMania::Palette0[0xDE] = SonicMania::ToRGB565(40, 32, 129);
// 
// 		drawDisp = 0.0f;
// 	}
// 	// Otherwise...
// 	else {
// 		SonicMania::ProcessAnimation(&UFOLamp);
// 		SonicMania::ProcessAnimation(&TVidle);
// 
// 		drawDisp -= 1.0f / 8.0f;
// 	}
// }
// 
// // ----------------------------------------------------------------------------
// // Drawing DOOM Mode's diorama on the main menu
// // ----------------------------------------------------------------------------
// void UIDiorama_Draw_DOOMMode(void) {
// 	SonicMania::ActiveEntityInfo* ActiveEnt = (SonicMania::ActiveEntityInfo*)&SonicMania::EntityInfo->CurrentEntity;
// 	SonicMania::Vector2 DrawPos;
// 
// 	DrawPos.X = ActiveEnt->CurrentEntity->Base.Position.X;
// 	DrawPos.Y = ActiveEnt->CurrentEntity->Base.Position.Y;
// 
// 	// If the background is being drawn...
// 	if (ActiveEnt->field_10 == ActiveEnt->CurrentEntity->Base.DrawOrder) {
// 		SonicMania::EntityAnimationData* ayYo = (SonicMania::EntityAnimationData*)&ActiveEnt->CurrentEntity->Extra[0x124];
// 
// 		// Draw the UFO lamps
// 		SonicMania::Vector2 UFOLampPos = { (ActiveEnt->CurrentEntity->Base.Position.X) + 6, (ActiveEnt->CurrentEntity->Base.Position.Y) - 41 };
// 		SonicMania::DrawSprite(&UFOLamp, &UFOLampPos, false);
// 		UFOLampPos = { (ActiveEnt->CurrentEntity->Base.Position.X) + 136, (ActiveEnt->CurrentEntity->Base.Position.Y) - 89 };
// 		int back = UFOLamp.CurrentFrame;
// 		UFOLamp.CurrentFrame = ((UFOLamp.CurrentFrame - 12) & 0xF);
// 		SonicMania::DrawSprite(&UFOLamp, &UFOLampPos, false);
// 		UFOLamp.CurrentFrame = back;
// 
// 		// Draw Studiopolis's FG and BG
// 		ayYo->CurrentFrame = 4;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 
// 		ayYo->CurrentFrame = 0;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		ayYo->CurrentFrame = 2;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 
// 		DrawPos.X += (short)drawDisp % 1024;
// 		ayYo->CurrentFrame = 1;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		ayYo->CurrentFrame = 3;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		// If the end of the cloud strips is on screen, draw them a second time to loop around
// 		if (DrawPos.X < (ActiveEnt->CurrentEntity->Base.Position.X) - 1024 + 424) {
// 			DrawPos.X += 1024;
// 			ayYo->CurrentFrame = 1;
// 			SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 			ayYo->CurrentFrame = 3;
// 			SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		}
// 	}
// 	// Otherwise, for the foreground elements...
// 	else {
// 		SonicMania::EntityAnimationData* ayYo = (SonicMania::EntityAnimationData*) & ActiveEnt->CurrentEntity->Extra[0x124];
// 
// 		// Draw the TV
// 		ActiveEnt->CurrentEntity->Base.InkEffect = SonicMania::Ink_None;
// 		DrawPos.X = ActiveEnt->CurrentEntity->Base.Position.X;
// 		ayYo->CurrentFrame = 6;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		ayYo->CurrentFrame = 7;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		ActiveEnt->CurrentEntity->Base.InkEffect = SonicMania::Ink_Add;
// 		ActiveEnt->CurrentEntity->Base.Alpha = 128;
// 		SonicMania::DrawSprite(&TVidle, &DrawPos, false);
// 		ayYo->CurrentFrame = 8;
// 		ActiveEnt->CurrentEntity->Base.Alpha = 255;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 		ActiveEnt->CurrentEntity->Base.InkEffect = SonicMania::Ink_None;
// 
// 		// Draw Tails
// 		ayYo->CurrentFrame = 5;
// 		SonicMania::DrawSprite(ayYo, &DrawPos, false);
// 	}
// }
// 
// // ----------------------------------------------------------------------------
// // Sets the UIDiorama to draw the DOOM mode diorama when selected
// // ----------------------------------------------------------------------------
// __declspec(naked) static void UIDiorama_Swap_DOOM(void) {
// 	__asm {
// 		pop		edi
// 		mov		dword ptr[esi + 0x70], offset UIDiorama_Draw_DOOMMode
// 		mov		dword ptr[esi + 0x6C], offset UIDiorama_Update_DOOMMode
// 		pop		esi
// 		retn
// 	}
// }
// 
// // ----------------------------------------------------------------------------
// // Defines which palette set to use for each diorama in order of ID
// // ----------------------------------------------------------------------------
// _declspec(align(0x4)) const int UIDiorama_Pal_Table[] = {
// 	0x0, 0xC, 0xC, 0x1, 0x3, 0xF, 0xD, 0xE, 0x2
// };
// 
// // ----------------------------------------------------------------------------
// // Sets what exactly the DOOM MODE button does
// // ----------------------------------------------------------------------------
// const int loc_46880 = baseAddress + 0x046880, loc_4670C = baseAddress + 0x04670C, loc_46770 = baseAddress + 0x046770;
// __declspec(naked) static void MainMenu_SetButtFunc(void) {
// 	__asm {
// 		cmp		dword ptr[ecx + 0x110], MENUBUTTON_DOOMMODE
// 		je		setDoom
// 		mov		eax, loc_46880
// 		mov		dword ptr[ecx + 0x64], eax
// 		jmp		loc_4670C
// 	setDoom:
// 		mov		eax, DoomMode_Init
// 		mov		dword ptr[ecx + 0x64], eax
// 		jmp		loc_4670C
// 	}
// }

#pragma runtime_checks( "", off )

void DoomExited()
{
	doomed = false;
	SetFramebufferEnabled(false);
}

void LaunchDoom()
{
	doomed = D_DoomMain();
	if (doomed)
	{
		SetFramebufferEnabled(true);
		DoomMode_Init();
		I_AtExit(&DoomExited, false);
	}
}

#pragma runtime_checks( "", restore )

extern "C"
{
	__declspec(dllexport) void TheInitFunc()
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

		LaunchDoom();

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

		//delete gPatternDetector;


		//while (1)
		//{
		//	DoomMode_Main();
		//	
		//	if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_REALTIME)
		//		while (!FrameLimiter::Sync_RT());
		//	else if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
		//		while (!FrameLimiter::Sync_SLP());
		//}
		

		// If DOOM successfully initialized and is ready to go...
		// if (doomed) {
		// 	// Add the DOOM engine state
		// 	WriteData((char*)(baseAddress + 0x1C7CF6), (char)0xD);							// Expand the jump table to have another entry
		// 	WriteData((long*)(baseAddress + 0x1C80BC), (long)DoomMode_Wrapper);				// Squeeze the DOOM mode pointer into the end of the table
		// 	WriteData((char*)(baseAddress + 0x1C7DAC), (char)0xE);							// Patch something meant to trigger the default case
		// 
		// 	// Add "DOOM MODE" to the main menu
		// 	WriteData((char*)(baseAddress + 0x0461BB), (char)0x8);							// Patch the diorama update to include the DOOM button
		// 	WriteData((long*)(baseAddress + 0x0461C1), (long)MainMenu_LateUpdate_JmpTable);	// ...
		// 	WriteJump((void*)(baseAddress + 0x046700), MainMenu_SetButtFunc);				// Patch in button functionality
		// 
		// 	// Add "DOOM MODE"s diorama
		// 	WriteData((char*)(baseAddress + 0x0442B9), (char)0x8);							// Patch the diorama swap table
		// 	WriteData((long*)(baseAddress + 0x044398), (long)UIDiorama_Swap_DOOM);			// ...
		// 
		// 	WriteData((long*)(baseAddress + 0x04427E), (long)UIDiorama_Pal_Table);			// Adds palette ID entry for DOOM mode's diorama when swapping
		// }

		// WriteJump((void*)(baseAddress + 0x0464F0), MainMenu_ButtInit);						// Regardless of if DOOM loaded or not, we're going to handle menu button initialization ourselves
	}

	// __declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}