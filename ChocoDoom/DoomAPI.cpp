#include "pch.h"
#include "DoomAPI.h"
#include "DoomScreenTexture.h"
#include "DoomWin32.h"
#include "MainExports.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include "Chocolate DOOM/g_game.h"
#ifdef __cplusplus
}
#endif

DOOMAPI uint32_t* DoomAPI_DoomScreenTexture_GetScreenFramebuffer()
{
	return DoomScreenTexture::GetScreenFramebuffer();
}

DOOMAPI uint32_t DoomAPI_DoomScreenTexture_GetScreenHeight()
{
	return DoomScreenTexture::GetScreenHeight();
}

DOOMAPI uint32_t DoomAPI_DoomScreenTexture_GetScreenWidth()
{
	return DoomScreenTexture::GetScreenWidth();
}

DOOMAPI bool DoomAPI_bIsDoomRunning()
{
	return bIsDoomRunning();
}

DOOMAPI bool DoomAPI_bHasDoomErrored()
{
	return bHasDoomErrored();
}

DOOMAPI bool DoomAPI_LaunchDoom(const char* args)  // #TODO: expand for wad selection
{
	return LaunchDoom(args);
}

DOOMAPI void DoomAPI_SetModPath(const char* path)
{
	return SetModPath(path);
}

DOOMAPI void DoomAPI_DoomMainLoopFunc()
{
	return DoomMainLoopFunc();
}

DOOMAPI void DoomAPI_DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error)
{
	return DoomRegisterAtExit(func, run_if_error);
}

DOOMAPI void DoomAPI_SetHWND(HWND hWnd)
{
	return DoomWin32_SetHWND(hWnd);
}

DOOMAPI HWND DoomAPI_GetHWND()
{
	return DoomWin32_GetHWND();
}

DOOMAPI int DoomAPI_GetCompletedLevels()
{
	return GetCompletedLevels();
}