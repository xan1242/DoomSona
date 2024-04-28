// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

extern "C" {
#include "Chocolate DOOM/z_zone.h"
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_DETACH:
        Z_Deinit();
        break;
    }
    return TRUE;
}

