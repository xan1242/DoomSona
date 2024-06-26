//
// Copyright(C) 2020 TheStoneBanana
// Copyright(C) 2024 Tenjoin / xan1242
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Chocolate Doom library entrypoint
//

#include "pch.h"
#include "MainExports.hpp"
#include <vector>
#include <string>
#include <sstream>

#include <chrono>
#include <thread>


extern "C" {
#include "Chocolate DOOM/d_main.h"
#include "Chocolate DOOM/z_zone.h"
#include "Chocolate DOOM/i_system.h"
#include "Chocolate DOOM/m_argv.h"
}

//const char* modPath;
std::filesystem::path realModPath;
std::string strModPath;
//char* modPathBuf;

bool doomed = false;
bool doomedonce = false;
bool about_to_exit = false;
bool bDoomErrored = false;


std::vector<char*> sArgs;

// ----------------------------------------------------------------------------
// DOOM's engine state
// ----------------------------------------------------------------------------

void DoomMode_Main(void) 
{
	D_DoomLoop();
}

// ----------------------------------------------------------------------------
// Preparing to meet our doom
// ----------------------------------------------------------------------------
static void DoomMode_Init(void) 
{
	D_DoomInit_Lite();
}


//#pragma runtime_checks( "", off )

bool bIsDoomRunning()
{
	return doomed;
}

bool bHasDoomErrored()
{
    return bDoomErrored;
}

int bDoomIsAboutToExit()
{
    return about_to_exit;
}

constexpr DWORD DoomExitDelay = 1300;
void DoomDelayedExit()
{
    Sleep(DoomExitDelay);
    I_Quit();
}

void DoomDelayedExitSpinup()
{
    about_to_exit = true;
    std::thread threadPostDelay(DoomDelayedExit);
    threadPostDelay.detach();
}

void DoomExited()
{
    for (char* s : sArgs)
    {
        if (s)
            free(s);
    }

    sArgs.clear();
    sArgs.shrink_to_fit();

    realModPath.clear();
    strModPath.clear();
    
    strModPath.shrink_to_fit();

    doomed = false;
}

//
// throw C++ exception from C land...?
// ...or maybe not. this actually can't be handled externally...
// this seems to work though
//
void DoomErrored(const char* str)
{
    printf("DOOM error!\n");
    bDoomErrored = true;
    //throw std::runtime_error(str);
    //DebugBreak();
}

std::vector<char*> parseArguments(const std::string& argsString)
{
    std::vector<char*> arguments;
    std::istringstream iss(argsString);
    std::string argument;

    //constexpr const char* dummyArg = "dummy";
    //const char* dummyArg = modPath;
    std::string firstArg = GetModPath();
    firstArg += "/ChocoDoom.dll";

    char* staticStr = (char*)malloc(strlen(firstArg.c_str()) + 1);
    memset(staticStr, 0, strlen(firstArg.c_str()) + 1);
    memcpy(staticStr, firstArg.c_str(), strlen(firstArg.c_str()));

    arguments.push_back(staticStr);

    while (std::getline(iss, argument, ' '))
    {
        if (!argument.empty() && (argument.front() == '"' || argument.front() == '\''))
        {
            // Handle quoted arguments
            char quoteChar = argument.front();
            std::string quotedArgument = argument.substr(1); // Remove opening quote

            // Keep reading until closing quote or end of string
            while (iss >> argument)
            {
                quotedArgument += " " + argument;
                if (argument.back() == quoteChar)
                {
                    // Remove closing quote
                    quotedArgument.pop_back();
                    break;
                }
            }

            staticStr = (char*)malloc(quotedArgument.size() + 1);
            memset(staticStr, 0, quotedArgument.size() + 1);
            memcpy(staticStr, quotedArgument.c_str(), quotedArgument.size());

            arguments.push_back(staticStr);
        }
        else
        {
            staticStr = (char*)malloc(argument.size() + 1);
            memset(staticStr, 0, argument.size() + 1);
            memcpy(staticStr, argument.c_str(), argument.size());

            arguments.push_back(staticStr);
        }
    }

    return arguments;
}

// #TODO: expand for wad selection
bool LaunchDoom(const char* args)
{
    if (!doomedonce)
    {
        sArgs.clear();
        sArgs = parseArguments(args);

        myargc = sArgs.size();
        myargv = &sArgs[0];

        doomed = D_DoomMain();
    }
	else
	{
		doomed = true;
	}
	if (doomed)
	{
		doomedonce = doomed;
		DoomMode_Init();
		I_AtExit(&DoomExited, true);
	}

	return doomed && !bDoomErrored;
}

void DoomMainLoopFunc()
{
	return DoomMode_Main();
}

//void SetModPath(const char* path)
//{
//	modPath = path;
//}

void SetModPath(std::filesystem::path path)
{
    realModPath = path;
    strModPath = path.string();
}

const char* GetModPath()
{
    return strModPath.c_str();
}

std::filesystem::path GetRealModPath()
{
    return realModPath;
}

void DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error)
{
	I_AtExit(func, run_if_error);
	return;
}

//#pragma runtime_checks( "", restore )
