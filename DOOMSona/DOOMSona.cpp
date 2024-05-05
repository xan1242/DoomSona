//
// DOOMSona
// A Persona 5 Royal to DOOM hook & bridge
//

#include "framework.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <fstream>
#include "DOOMSona.hpp"
#include "DoomD3DHook.hpp"
#include "DoomAPIIntegration.hpp"
//#include "ReloadedPathIntegration.hpp"
#include "Installer.hpp"
#include "ModPath.hpp"

#include "includes/injector/injector.hpp"
#include "includes/assembly64.hpp"

#include "dxhook/src/MemoryUtils.h"

namespace DOOMSona
{
    int numCompletedLevelsBySkill[sk_count];
    int numCompletedLevels;
    bool bFinishedShareware;
    uint32_t personaCurrBGM;

    bool bUndoCBT;

    std::filesystem::path thisModPath;
    //std::filesystem::path rldModPath;
    std::filesystem::path currRootPath;

    //
    // Explanation:
    // We have to hijack DBG_PUTS script function because that is the only one of two script functions
    // that accept a string parameter (with the other being PUTS).
    // 
    // So, a keyword at the beginning is used to denote that it is being used to apply the remainder of 
    // the string after the keyword as the current string parameter that we want to use in the next function call (TBL_365_VALUE_MD).
    // 
    // Example:
    // To perform a file existence check, in the GFD flow script you'd do the following:
    // 
    // // applies the currStringParam with the given path
    // DBG_PUTS("CACODEMON path/to/my/file");
    // 
    // // executes the hooked function with DPSP_FILEEXISTS, which performs a std::filesystem::exists and returns a value (1 = exists, 0 = doesn't exist)
    // // assuming DoomPersonaScriptParams enum is also in the flow script
    // int result = TBL_365_VALUE_MD(DoomPersonaScriptParams.DPSP_FILEEXISTS, 0 ,0);
    //
    STRCONST std::string StringParamKeyword = "CACODEMON ";
    STRCONST const char* currModPathStr = "DOOMSona";
    STRCONST std::string argsFilename = "args.txt";
    const std::string overrideArgsFilename = "override-args.txt";

    std::string currStringParam;
    std::string currChocoDoomArgs = "";

    namespace DOOM
    {
        static void PerformAtDoomExit()
        {
            if (!DoomAPI::bHasDoomErrored())
            {
                bFinishedShareware = DoomAPI::GetFinishedShareware();
                numCompletedLevels = DoomAPI::GetUniquelyCompletedLevels();

                for (int i = 0; i < sk_count; i++)
                {
                    numCompletedLevelsBySkill[i] = DoomAPI::GetUniquelyCompletedLevelsBySkill(i);
                }

            }
            DoomD3DHook::SetFramebufferEnabled(false);
        }

        static void ResetDoomStats()
        {
            numCompletedLevels = 0;
            for (int i = 0; i < sk_count; i++)
            {
                numCompletedLevelsBySkill[i] = 0;
            }
            bFinishedShareware = false;
        }

        static void ReadLaunchParamsOptFile(std::filesystem::path pathArgs, bool bOverride)
        {
            try
            {
                if (!std::filesystem::exists(pathArgs))
                    return;
            }
            catch (...)
            {
                return;
            }

            std::ifstream ifile;
            try
            {
                ifile.open(pathArgs);
            }
            catch (...)
            {
                return;
            }

            if (!ifile.is_open())
                return;

            std::string line;
            std::string collectedArgs;

            while (std::getline(ifile, line))
            {
                collectedArgs += line + ' ';
            }

            ifile.close();

            if (currChocoDoomArgs.empty() || bOverride)
                currChocoDoomArgs = collectedArgs;
            else if (currChocoDoomArgs != collectedArgs)
                currChocoDoomArgs += collectedArgs;
        }

        static bool Launch()
        {
            std::filesystem::path baseDoomSona = currRootPath;
            baseDoomSona /= currModPathStr;

            if (!DoomAPI::modHandle)
            {
                std::filesystem::path libDoomPath = baseDoomSona;
                libDoomPath /= "ChocoDoom.dll";
                if (!DoomAPI::Init(libDoomPath))
                    return false;
            }


            DoomAPI::SetModPathW(baseDoomSona.wstring().c_str());

            // read override-args.txt -- overrides anything set by the GFD script
            std::filesystem::path pathArgs = currRootPath;
            pathArgs /= currModPathStr;
            pathArgs /= overrideArgsFilename;
            ReadLaunchParamsOptFile(pathArgs, true);

            // read args.txt
            pathArgs = currRootPath;
            pathArgs /= currModPathStr;
            pathArgs /= argsFilename;
            ReadLaunchParamsOptFile(pathArgs, false);

            DoomD3DHook::SetFramebufferEnabled(true);

            printf("DOOMSona: Launching DOOM with args: %s\n", currChocoDoomArgs.c_str());

            if (!DoomAPI::LaunchDoom(currChocoDoomArgs.c_str()))
                return false;

            ResetDoomStats();

            DoomAPI::DoomRegisterAtExit(PerformAtDoomExit, true);

            return true;
        }
    }

    static bool separate_console(void)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
            return false;

        return true;
    }

    static void OpenConsole()
    {
        if (!separate_console())
        {
            AllocConsole();
            AttachConsole(ATTACH_PARENT_PROCESS);

            freopen("CON", "wb", stdout);
            freopen("CON", "wb", stderr);
        }
    }

    // Persona hooks

    namespace GFD
    {
        constexpr uintptr_t pGetScriptArg = 0x1416E83F0;
        constexpr uintptr_t pScriptContext = 0x142ADCC28;

        static uintptr_t GetScriptContext()
        {
            return *reinterpret_cast<uintptr_t*>(pScriptContext);
        }

        static uint32_t _stdcall GetScriptArg(int num)
        {
            return reinterpret_cast<uint32_t(__thiscall*)(int)>(pGetScriptArg)(num);
        }

        static char* GetScriptArgStr(int num)
        {
            uintptr_t context = GetScriptContext();
            uint32_t argCount = *reinterpret_cast<uint32_t*>(context + 0x2C);

            uint8_t argType = *reinterpret_cast<uint8_t*>(context + 0x2F + (argCount - num));

            if (argType != 5)
                return nullptr;


            return *reinterpret_cast<char**>(context + 0x58 + (sizeof(uint64_t) * (argCount - num)));
        }

        static void SetReturnVal(uint64_t val)
        {
            uintptr_t context = GetScriptContext();

            // return value
            *reinterpret_cast<uint64_t*>(context + 0x1D8) = val;
            *reinterpret_cast<uint8_t*>(context + 0x5F) = 0;
        }
    }


    static uint32_t _stdcall CheckDisableSharePlayHook()
    {
        //uint64_t arg0 = GFD::GetScriptArg(0);

        bool bDoomLaunchResult = false;

        if (!DoomAPI::bIsDoomRunning())
            bDoomLaunchResult = DOOM::Launch();

        GFD::SetReturnVal(bDoomLaunchResult);

        return 1;
    }

    // multi-tool function - get stats and crap from game by input parameter
    static uint32_t _stdcall TBL365ValueMDHook()
    {
        //uint32_t arg2 = GFD::GetScriptArg(2);
        uint32_t arg1 = GFD::GetScriptArg(1);
        uint32_t arg0 = GFD::GetScriptArg(0);

        uint64_t retval = 0;

        std::filesystem::path chkPath = currRootPath;

        switch (arg0)
        {
        case DPSP_FINISHEDSHAREWARE:
            retval = bFinishedShareware;
            break;
        case DPSP_COMPLETEDLEVELSBYSKILL:
            retval = numCompletedLevelsBySkill[arg1];
            break;
        case DPSP_COMPLETEDLEVELS:
            retval = numCompletedLevels;
            break;
        case DPSP_APPLYDOOMARGS:
            currChocoDoomArgs = currStringParam;
            currStringParam.clear();
            break;
        case DPSP_FILESIZE:
            if (std::filesystem::path(currRootPath).is_absolute())
                chkPath = currRootPath;
            else
                chkPath /= currRootPath;
            try
            {
                retval = std::filesystem::file_size(chkPath);
            }
            catch (const std::exception& e)
            {
                retval = 0;
            }
            currStringParam.clear();
            break;
        case DPSP_FILEEXISTS:
            if (std::filesystem::path(currRootPath).is_absolute())
                chkPath = currRootPath;
            else
                chkPath /= currRootPath;
            try
            {
                retval = std::filesystem::exists(chkPath);
            }
            catch (const std::exception& e)
            {
                retval = 0;
            }
            currStringParam.clear();
            break;
        case DPSP_CURRBGM:
            retval = personaCurrBGM;
            break;
        case DPSP_ISGAMERUNNING:
            retval = DoomAPI::bIsDoomRunning();
            break;
        default:
            break;
        }

        GFD::SetReturnVal(retval);

        return 1;
    }

    static bool bIncludesStrParamKeyword(const char* str)
    {
        if (!str)
            return false;

        std::string inString = str;
        if (inString.empty())
            return false;

        if (inString.length() < StringParamKeyword.length())
            return false; // The string is shorter than the keyword

        std::transform(inString.begin(), inString.end(), inString.begin(), ::toupper);

        for (size_t i = 0; i < StringParamKeyword.length(); ++i)
        {
            if (inString[i] != StringParamKeyword[i])
            {
                return false; // Characters don't match
            }
        }

        return true;
    }

    static void CopyStringParam(const char* str)
    {
        std::string inString = str;
        currStringParam = inString.substr(StringParamKeyword.length());
    }

    static uint32_t _stdcall DbgPutsHook()
    {
        //uintptr_t context = *reinterpret_cast<uintptr_t*>(pScriptContext);

        //MemoryUtils::PrintBytesAtAddress(context, 0x200);

        char* in_str = GFD::GetScriptArgStr(0);

        if (in_str)
        {
            if (bIncludesStrParamKeyword(in_str))
            {
                CopyStringParam(in_str);
                //printf("CACODEMON is: %s\n", currStringParam.c_str());
            }
            else
                printf("DBG_PUTS: %s\n", in_str);
        }

        return 1;
    }

    // static void GetPathFromReloadedModule()
    // {
    //     int c = 0;
    // 
    //     // wait for the module to appear
    //     while (!ReloadedPath::modHandle)
    //     {
    //         ReloadedPath::Init("Reloaded-DOOMSonaLib.dll");
    //         Sleep(1);
    //         c++;
    // 
    //         // stop after 5s
    //         if (c >= 5000)
    //             break;
    //     }
    // 
    //     if (!ReloadedPath::modHandle)
    //     {
    //         return;
    //     }
    // 
    //     size_t len = (ReloadedPath::GetModPathLength() + 1) * sizeof(char16_t);
    //     char16_t* tmp = (char16_t*)(malloc(len));
    //     if (tmp != nullptr)
    //     {
    //         memset(tmp, 0, len);
    //         ReloadedPath::GetModPathW(tmp);
    //         rldModPath = tmp;
    //         free(tmp);
    // 
    //         currRootPath = rldModPath.parent_path();
    //     }
    // }

    void Init()
    {
        thisModPath = ModPath::GetThisModulePath<std::filesystem::path>();
        currRootPath = thisModPath.parent_path();

        OpenConsole();
        Installer::CleanupOldFiles();
        DoomD3DHook::Init();

        struct hkGameLoop
        {
            void operator()(injector::reg_pack& regs)
            {
                // Have to undo what the CBT mod does because it doesn't hook this functions properly...
                // and to be fair, neither are we here...
                // #TODO: pass call to original func!
                if (!bUndoCBT)
                {
                    bUndoCBT = true;
                    injector::WriteMemory<uintptr_t>(0x0000000141AA7270, 0x1416DD550, true);
                }

                if (DoomAPI::bIsDoomRunning() && !DoomAPI::bHasDoomErrored())
                {
                    regs.rax = 0;
                    DoomAPI::DoomMainLoopFunc();
                }
                else
                {
                    if (DoomAPI::modHandle)
                    {
                        DoomAPI::Deinit();
                    }
                    regs.rax = *reinterpret_cast<uintptr_t*>(0x142A0B720);
                }
            }
        }; injector::MakeInline<hkGameLoop>(0x1401D6800, 0x1401D6800 + 7);

        //
        // The game doesn't seem to have easy access to this info via scripting, so this script is done to ease access to it.
        // Simply stores the last played BGM ID and saves it for later use.
        // Can be obtained via the multi-tool function via DPSP_CURRBGM.
        //
        struct hkCurrBGM
        {
            void operator()(injector::reg_pack& regs)
            {
                personaCurrBGM = regs.rbx;
                regs.rax = 0x141AAA670;
            }
        }; injector::MakeInline<hkCurrBGM>(0x15590AB38, 0x15590AB3F);

        // #TODO: not proper hooks, hook them cleanly at high-level and pass the call to the next function
        injector::MakeAbsJMP(0x00000001416E1D20, CheckDisableSharePlayHook);
        injector::MakeAbsJMP(0x00000001416DDFE0, TBL365ValueMDHook);
        injector::MakeAbsJMP(0x00000001416DD550, DbgPutsHook);
        // increase the cmmBookTable limit to 127 (this is as much as we can do without hooking code since it's a 'cmp esi, 7Fh' here)
        injector::WriteMemory<uint8_t>(0x0000000140C8993B + 2, 0x7F, true); 

        // spin up a thread to wait for the reloaded module...
        // std::thread rldWaitThread(GetPathFromReloadedModule);
        // rldWaitThread.detach();
    }
}