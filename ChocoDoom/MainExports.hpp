#pragma once

#ifndef MAINEXPORTS_HPP
#define MAINEXPORTS_HPP

#ifdef __cplusplus

typedef void (*doom_atexit_func_t)(void);

bool bIsDoomRunning();
bool bHasDoomErrored();
bool LaunchDoom(const char* args);
void SetModPath(const char* path);
void DoomMainLoopFunc();
void DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error);

#endif

#ifdef __cplusplus
extern "C" {
#endif

void DoomErrored(const char* str);

#ifdef __cplusplus
}
#endif

#endif