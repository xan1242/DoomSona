#pragma once
#define USE_MATH_DEFINES
#define NOMINMAX
#include "framework.h"
#include <iostream>
#include <filesystem>

#ifndef DOOMAPIINTEGRATION_HPP
#define DOOMAPIINTEGRATION_HPP

#define DOOMAPI extern "C" __declspec(dllexport)

namespace DoomAPI
{
	inline HMODULE modHandle;
	typedef void (*atexit_func_t)(void);


	inline bool(*_bIsDoomRunning)();
	inline bool bIsDoomRunning()
	{
		if (!_bIsDoomRunning && modHandle)
			_bIsDoomRunning = reinterpret_cast<bool(*)()>(GetProcAddress(modHandle, "DoomAPI_bIsDoomRunning"));

		if (_bIsDoomRunning)
			return _bIsDoomRunning();

		return false;
	}

	inline bool(*_LaunchDoom)(); // #TODO: expand for wad selection
	inline bool LaunchDoom()
	{
		if (!_LaunchDoom && modHandle)
			_LaunchDoom = reinterpret_cast<bool(*)()>(GetProcAddress(modHandle, "DoomAPI_LaunchDoom"));

		if (_LaunchDoom)
			return _LaunchDoom();

		return false;
	}

	inline void(*_SetModPath)(const char* path);
	inline void SetModPath(const char* path)
	{
		if (!_SetModPath && modHandle)
			_SetModPath = reinterpret_cast<void(*)(const char*)>(GetProcAddress(modHandle, "DoomAPI_SetModPath"));

		if (_SetModPath)
			return _SetModPath(path);
	}

	inline void(*_DoomMainLoopFunc)();
	inline void DoomMainLoopFunc()
	{
		if (!_DoomMainLoopFunc && modHandle)
			_DoomMainLoopFunc = reinterpret_cast<void(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomMainLoopFunc"));

		if (_DoomMainLoopFunc)
			return _DoomMainLoopFunc();
	}

	inline void(*_DoomRegisterAtExit)(atexit_func_t func, bool run_if_error);
	inline void DoomRegisterAtExit(atexit_func_t func, bool run_if_error)
	{
		if (!_DoomRegisterAtExit && modHandle)
			_DoomRegisterAtExit = reinterpret_cast<void(*)(atexit_func_t, bool)>(GetProcAddress(modHandle, "DoomAPI_DoomRegisterAtExit"));

		if (_DoomRegisterAtExit)
			return _DoomRegisterAtExit(func, run_if_error);
	}

	inline void(*_SetHWND)(HWND hWnd);
	inline void SetHWND(HWND hWnd)
	{
		if (!_SetHWND && modHandle)
			_SetHWND = reinterpret_cast<void(*)(HWND)>(GetProcAddress(modHandle, "DoomAPI_SetHWND"));

		if (_SetHWND)
			return _SetHWND(hWnd);
	}

	inline HWND(*_GetHWND)();
	inline HWND GetHWND()
	{
		if (!_GetHWND && modHandle)
			_GetHWND = reinterpret_cast<HWND(*)()>(GetProcAddress(modHandle, "DoomAPI_GetHWND"));

		if (_GetHWND)
			return _GetHWND();

		return 0;
	}

	namespace DoomScreenTexture
	{
		inline uint32_t* (*_GetScreenFramebuffer)();
		inline uint32_t* GetScreenFramebuffer()
		{
			if (!_GetScreenFramebuffer && modHandle)
				_GetScreenFramebuffer = reinterpret_cast<uint32_t * (*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenFramebuffer"));

			if (_GetScreenFramebuffer)
				return _GetScreenFramebuffer();

			return nullptr;
		}

		inline uint32_t(*_GetScreenHeight)();
		inline uint32_t GetScreenHeight()
		{
			if (!_GetScreenHeight && modHandle)
				_GetScreenHeight = reinterpret_cast<uint32_t(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenHeight"));

			if (_GetScreenHeight)
				return _GetScreenHeight();

			return 0;
		}

		inline uint32_t(*_GetScreenWidth)();
		inline uint32_t GetScreenWidth()
		{
			if (!_GetScreenWidth && modHandle)
				_GetScreenWidth = reinterpret_cast<uint32_t(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenWidth"));

			if (_GetScreenWidth)
				return _GetScreenWidth();

			return 0;
		}
	}

	inline bool Init(std::filesystem::path name)
	{
		modHandle = GetModuleHandle(name.filename().wstring().c_str());
		if (modHandle)
			FreeLibrary(modHandle);

		modHandle = LoadLibrary(name.wstring().c_str());
		if (!modHandle)
			return false;

		//printf("DoomAPI::Init\n");

		return true;
	}

	inline void Init(HMODULE mod)
	{
		modHandle = mod;
	}

	inline void Deinit()
	{
		if (!modHandle)
			return;

		//printf("DoomAPI::Deinit\n");

		FreeLibrary(modHandle);

		_bIsDoomRunning = NULL;
		_LaunchDoom = NULL;
		_SetModPath = NULL;
		_DoomMainLoopFunc = NULL;
		_DoomRegisterAtExit = NULL;
		_GetHWND = NULL;
		_SetHWND = NULL;

		DoomScreenTexture::_GetScreenFramebuffer = NULL;
		DoomScreenTexture::_GetScreenHeight = NULL;
		DoomScreenTexture::_GetScreenWidth = NULL;

		modHandle = NULL;
	}
}

#endif