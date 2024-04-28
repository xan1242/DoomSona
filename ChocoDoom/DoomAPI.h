#pragma once
#include "pch.h"
#include "stdint.h"
#include "MainExports.hpp"

#ifndef DOOMAPI_H
#define DOOMAPI_H

#define DOOMAPI extern "C" __declspec(dllexport)

DOOMAPI uint32_t* DoomAPI_DoomScreenTexture_GetScreenFramebuffer();
DOOMAPI uint32_t DoomAPI_DoomScreenTexture_GetScreenHeight();
DOOMAPI uint32_t DoomAPI_DoomScreenTexture_GetScreenWidth();
DOOMAPI bool DoomAPI_bIsDoomRunning();
DOOMAPI bool DoomAPI_LaunchDoom();  // #TODO: expand for wad selection
DOOMAPI void DoomAPI_SetModPath(const char* path);
DOOMAPI void DoomAPI_DoomMainLoopFunc();
DOOMAPI void DoomAPI_DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error);
DOOMAPI void DoomAPI_SetHWND(HWND hWnd);
DOOMAPI HWND DoomAPI_GetHWND();

#endif