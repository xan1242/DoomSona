#include "framework.h"
#include "Installer.hpp"
#include "PathReporter.hpp"

RLD_DOOMSONA_API void ReloadedStart()
{
    Installer::Init();
    PathReporter::Init();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}

