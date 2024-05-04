#include "framework.h"
#include "Installer.hpp"

extern "C" __declspec(dllexport) void ReloadedStart()
{
    Installer::Init();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}

