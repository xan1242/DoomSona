#pragma once
#include <windows.h>

#ifndef DOOMWIN32_H
#define DOOMWIN32_H

#ifdef __cplusplus
extern "C" {
#endif
	void __declspec(dllexport) DoomWin32_SetHWND(HWND hWnd);
	HWND __declspec(dllexport) DoomWin32_GetHWND();
	int TranslateWin32Key(int vkey);
#ifdef __cplusplus
}
#endif

#endif