// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// #include "ManiaModLoader.h"
// #include "SonicMania.h"

// namespace SonicMania {
// 	DataArray(unsigned short, Framebuffer, 0x6ECA88, 432 * 240);		// 424x240 display
// 	FunctionPointer(void, Engine_ClearSoundFXData, (), 0x1BC090);
// 
//     struct EntityUIButtonz : Entity
//     {
//         /* 0x00000058 */ char filler[0xB8];
//         DWORD FrameID;
//         char fill2[0x18];
//         DWORD Transition;
//         DWORD StopMusic;
//     };
// }