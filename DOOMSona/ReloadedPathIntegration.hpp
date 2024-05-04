#pragma once
#define USE_MATH_DEFINES
#define NOMINMAX
#include "framework.h"
#include <iostream>
#include <filesystem>

#ifndef RELOADEDPATHINTEGRATION_HPP
#define RELOADEDPATHINTEGRATION_HPP

#define RLD_DOOMSONA_API extern "C" __declspec(dllexport) 

namespace ReloadedPath
{
	inline HMODULE modHandle;

	inline size_t(*_GetModPathLength)();
	inline size_t GetModPathLength()
	{
		if (!_GetModPathLength && modHandle)
			_GetModPathLength = reinterpret_cast<size_t(*)()>(GetProcAddress(modHandle, "PathReporter_GetModPathLength"));

		if (_GetModPathLength)
			return _GetModPathLength();

		return 0;
	}

	inline const char* (*_GetModPathA)(char* outStr);
	inline const char* GetModPathA(char* outStr)
	{

		if (!_GetModPathA && modHandle)
			_GetModPathA = reinterpret_cast<const char* (*)(char*)>(GetProcAddress(modHandle, "PathReporter_GetModPathA"));

		if (_GetModPathA)
			return _GetModPathA(outStr);

		return nullptr;
	}

	inline const char16_t* (*_GetModPathW)(char16_t* outStr);
	inline const char16_t* GetModPathW(char16_t* outStr)
	{

		if (!_GetModPathW && modHandle)
			_GetModPathW = reinterpret_cast<const char16_t* (*)(char16_t*)>(GetProcAddress(modHandle, "PathReporter_GetModPathW"));

		if (_GetModPathW)
			return _GetModPathW(outStr);

		return nullptr;
	}

	inline bool Init(std::filesystem::path name)
	{
		modHandle = GetModuleHandle(name.filename().wstring().c_str());

		return modHandle != 0;
	}
}

#endif