#pragma once

#ifndef MAINEXPORTS_HPP
#define MAINEXPORTS_HPP

#ifdef __cplusplus

#include <filesystem>

typedef void (*doom_atexit_func_t)(void);

bool bIsDoomRunning();
bool bHasDoomErrored();
bool LaunchDoom(const char* args);
void SetModPath(std::filesystem::path path);
void DoomMainLoopFunc();
void DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error);
std::filesystem::path GetRealModPath();

#endif

#ifdef __cplusplus
extern "C" {
#endif

void DoomErrored(const char* str);
void DoomDelayedExitSpinup();
int bDoomIsAboutToExit();
const char* GetModPath();

#ifdef __cplusplus
}
#endif

#endif