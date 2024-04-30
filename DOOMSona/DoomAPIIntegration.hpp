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
		//printf("DoomAPI::bIsDoomRunning\n");

		if (!_bIsDoomRunning && modHandle)
			_bIsDoomRunning = reinterpret_cast<bool(*)()>(GetProcAddress(modHandle, "DoomAPI_bIsDoomRunning"));

		if (_bIsDoomRunning)
			return _bIsDoomRunning();

		return false;
	}

	inline bool(*_bHasDoomErrored)();
	inline bool bHasDoomErrored()
	{
		//printf("DoomAPI::bHasDoomErrored. 0x%llX\n", _bHasDoomErrored);

		if (!_bHasDoomErrored && modHandle)
			_bHasDoomErrored = reinterpret_cast<bool(*)()>(GetProcAddress(modHandle, "DoomAPI_bHasDoomErrored"));

		if (_bHasDoomErrored)
			return _bHasDoomErrored();

		return false;
	}

	inline bool(*_LaunchDoom)(const char* args);
	inline bool LaunchDoom(const char* args)
	{
		//printf("DoomAPI::LaunchDoom\n");

		if (!_LaunchDoom && modHandle)
			_LaunchDoom = reinterpret_cast<bool(*)(const char*)>(GetProcAddress(modHandle, "DoomAPI_LaunchDoom"));

		if (_LaunchDoom)
			return _LaunchDoom(args);

		return false;
	}

	inline void(*_SetModPath)(const char* path);
	inline void SetModPath(const char* path)
	{
		//printf("DoomAPI::SetModPath\n");

		if (!_SetModPath && modHandle)
			_SetModPath = reinterpret_cast<void(*)(const char*)>(GetProcAddress(modHandle, "DoomAPI_SetModPath"));

		if (_SetModPath)
			return _SetModPath(path);
	}

	inline void(*_DoomMainLoopFunc)();
	inline void DoomMainLoopFunc()
	{
		//printf("DoomAPI::DoomMainLoopFunc\n");

		if (!_DoomMainLoopFunc && modHandle)
			_DoomMainLoopFunc = reinterpret_cast<void(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomMainLoopFunc"));

		if (_DoomMainLoopFunc)
			return _DoomMainLoopFunc();
	}

	inline void(*_DoomRegisterAtExit)(atexit_func_t func, bool run_if_error);
	inline void DoomRegisterAtExit(atexit_func_t func, bool run_if_error)
	{
		//printf("DoomAPI::DoomRegisterAtExit\n");

		if (!_DoomRegisterAtExit && modHandle)
			_DoomRegisterAtExit = reinterpret_cast<void(*)(atexit_func_t, bool)>(GetProcAddress(modHandle, "DoomAPI_DoomRegisterAtExit"));

		if (_DoomRegisterAtExit)
			return _DoomRegisterAtExit(func, run_if_error);
	}

	inline void(*_SetHWND)(HWND hWnd);
	inline void SetHWND(HWND hWnd)
	{
		//printf("DoomAPI::SetHWND\n");

		if (!_SetHWND && modHandle)
			_SetHWND = reinterpret_cast<void(*)(HWND)>(GetProcAddress(modHandle, "DoomAPI_SetHWND"));

		if (_SetHWND)
			return _SetHWND(hWnd);
	}

	inline HWND(*_GetHWND)();
	inline HWND GetHWND()
	{
		//printf("DoomAPI::GetHWND\n");

		if (!_GetHWND && modHandle)
			_GetHWND = reinterpret_cast<HWND(*)()>(GetProcAddress(modHandle, "DoomAPI_GetHWND"));

		if (_GetHWND)
			return _GetHWND();

		return 0;
	}

	inline int(*_GetCompletedLevels)();
	inline int GetCompletedLevels()
	{
		if (!_GetCompletedLevels && modHandle)
			_GetCompletedLevels = reinterpret_cast<int(*)()>(GetProcAddress(modHandle, "DoomAPI_GetCompletedLevels"));

		if (_GetCompletedLevels)
			return _GetCompletedLevels();

		return 0;
	}

	namespace DoomScreenTexture
	{
		inline uint32_t* (*_GetScreenFramebuffer)();
		inline uint32_t* GetScreenFramebuffer()
		{
			//printf("DoomAPI::GetScreenFramebuffer\n");

			if (!_GetScreenFramebuffer && modHandle)
				_GetScreenFramebuffer = reinterpret_cast<uint32_t * (*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenFramebuffer"));

			if (_GetScreenFramebuffer)
				return _GetScreenFramebuffer();

			return nullptr;
		}

		inline uint32_t(*_GetScreenHeight)();
		inline uint32_t GetScreenHeight()
		{
			//printf("DoomAPI::GetScreenHeight\n");

			if (!_GetScreenHeight && modHandle)
				_GetScreenHeight = reinterpret_cast<uint32_t(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenHeight"));

			if (_GetScreenHeight)
				return _GetScreenHeight();

			return 0;
		}

		inline uint32_t(*_GetScreenWidth)();
		inline uint32_t GetScreenWidth()
		{
			//printf("DoomAPI::GetScreenWidth\n");

			if (!_GetScreenWidth && modHandle)
				_GetScreenWidth = reinterpret_cast<uint32_t(*)()>(GetProcAddress(modHandle, "DoomAPI_DoomScreenTexture_GetScreenWidth"));

			if (_GetScreenWidth)
				return _GetScreenWidth();

			return 0;
		}
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
		_bHasDoomErrored = NULL;
		_GetCompletedLevels = NULL;

		DoomScreenTexture::_GetScreenFramebuffer = NULL;
		DoomScreenTexture::_GetScreenHeight = NULL;
		DoomScreenTexture::_GetScreenWidth = NULL;

		modHandle = NULL;
	}

	inline bool Init(std::filesystem::path name)
	{
		modHandle = GetModuleHandle(name.filename().wstring().c_str());
		if (modHandle)
			Deinit();

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
}

#endif