#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h>

#ifndef DOOMAPI_HPP
#define DOOMAPI_HPP

namespace DoomAPI
{
	inline HMODULE modDoom;

	inline uint32_t*(*_GetScreenBuffer)();
	inline uint32_t* GetScreenBuffer()
	{
		return _GetScreenBuffer();
	}

	inline uint32_t (*_GetScreenX)();
	inline uint32_t GetScreenX()
	{
		return _GetScreenX();
	}

	inline uint32_t(*_GetScreenY)();
	inline uint32_t GetScreenY()
	{
		return _GetScreenY();
	}

	inline bool(*_GetFramebufferEnabled)();
	inline bool GetFramebufferEnabled()
	{
		return _GetFramebufferEnabled();
	}

	inline void(*_SetFramebufferEnabled)(bool val);
	inline void SetFramebufferEnabled(bool val)
	{
		return _SetFramebufferEnabled(val);
	}

	inline void(*_DoomWin32_SetHWND)(HWND wnd);
	inline void DoomWin32_SetHWND(HWND wnd)
	{
		return _DoomWin32_SetHWND(wnd);
	}

	inline HWND(*_DoomWin32_GetHWND)();
	inline HWND DoomWin32_GetHWND()
	{
		return _DoomWin32_GetHWND();
	}

	inline bool Init()
	{
		modDoom = LoadLibraryA("DOOMania.dll");
		if (!modDoom)
			return false;

		uintptr_t TheInitFunc = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "TheInitFunc"));
		if (!TheInitFunc)
			return false;

		uintptr_t pGetScreenBuffer = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "GetScreenBuffer"));
		if (!pGetScreenBuffer)
			return false;

		_GetScreenBuffer = reinterpret_cast<uint32_t*(*)()>(pGetScreenBuffer);

		uintptr_t pGetScreenX = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "GetScreenX"));
		if (!pGetScreenX)
			return false;

		_GetScreenX = reinterpret_cast<uint32_t(*)()>(pGetScreenX);

		uintptr_t pGetScreenY = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "GetScreenY"));
		if (!pGetScreenY)
			return false;

		_GetScreenY = reinterpret_cast<uint32_t(*)()>(pGetScreenY);

		uintptr_t pGetFramebufferEnabled = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "GetFramebufferEnabled"));
		if (!pGetFramebufferEnabled)
			return false;

		_GetFramebufferEnabled = reinterpret_cast<bool(*)()>(pGetFramebufferEnabled);

		uintptr_t pSetFramebufferEnabled = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "SetFramebufferEnabled"));
		if (!pSetFramebufferEnabled)
			return false;

		_SetFramebufferEnabled = reinterpret_cast<void(*)(bool)>(pSetFramebufferEnabled);

		uintptr_t pDoomWin32_SetHWND = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "DoomWin32_SetHWND"));
		if (!pDoomWin32_SetHWND)
			return false;

		_DoomWin32_SetHWND = reinterpret_cast<void(*)(HWND)>(pDoomWin32_SetHWND);

		uintptr_t pDoomWin32_GetHWND = reinterpret_cast<uintptr_t>(GetProcAddress(modDoom, "DoomWin32_GetHWND"));
		if (!pDoomWin32_GetHWND)
			return false;

		_DoomWin32_GetHWND = reinterpret_cast<HWND(*)()>(pDoomWin32_GetHWND);

		reinterpret_cast<void(*)()>(TheInitFunc)();

		return true;
	}

}

#endif