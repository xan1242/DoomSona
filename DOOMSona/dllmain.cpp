#include "framework.h"
#include "DOOMSona.hpp"
#include "UALCheck.hpp"

// has to be inited outside DllMain because of DXGI hooks...
extern "C" __declspec(dllexport) void InitializeASI()
{
    DOOMSona::Init();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    //switch (ul_reason_for_call)
    //{
    //case DLL_PROCESS_ATTACH:
    //    if (!UALCheck::IsUALPresent())
    //        InitializeASI();
    //    break;
    //}
    return TRUE;
}

