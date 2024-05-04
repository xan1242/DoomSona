#include "pch.h"
#include "DoomAPI.h"
#include "DoomScreenTexture.h"
#include "DoomWin32.h"
#include "MainExports.hpp"
#include "DoomLevelCompletionTracker.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "Chocolate DOOM/f_finale.h"
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

DOOMAPI bool DoomAPI_LaunchDoom(const char* args)
{
	return LaunchDoom(args);
}

DOOMAPI void DoomAPI_SetModPathA(const char* path)
{
	return SetModPath(path);
}

DOOMAPI void DoomAPI_SetModPathW(const wchar_t* path)
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

DOOMAPI int DoomAPI_GetUniquelyCompletedLevels()
{
	return DoomLevelCompletionTracker_GetUniqueMapCount();
}

DOOMAPI int DoomAPI_GetUniquelyCompletedLevelsBySkill(int skill)
{
	return DoomLevelCompletionTracker_GetUniqueMapCountBySkill(skill);
}

DOOMAPI bool DoomAPI_GetFinishedShareware()
{
	return F_GetSharewareFinished();
}