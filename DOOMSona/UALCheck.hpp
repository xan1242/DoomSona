#pragma once
#include "framework.h"
#include <iostream>
#include <stacktrace>

#ifndef UALCHECK_HPP
#define UALCHECK_HPP

namespace UALCheck
{
    inline bool IsModuleUAL(HMODULE mod)
    {
        if (GetProcAddress(mod, "IsUltimateASILoader") != NULL || (GetProcAddress(mod, "DirectInput8Create") != NULL && GetProcAddress(mod, "DirectSoundCreate8") != NULL && GetProcAddress(mod, "InternetOpenA") != NULL))
            return true;
        return false;
    }

    inline bool IsUALPresent()
    {
        for (const auto& entry : std::stacktrace::current())
        {
            HMODULE hModule = NULL;
            if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)entry.native_handle(), &hModule))
            {
                if (IsModuleUAL(hModule))
                    return true;
            }
        }
        return false;
    }
}

#endif
