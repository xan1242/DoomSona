#pragma once

#ifndef MAINEXPORTS_HPP
#define MAINEXPORTS_HPP

//#ifdef __cplusplus
//extern "C" {
//#endif

typedef void (*doom_atexit_func_t)(void);

bool bIsDoomRunning();
bool LaunchDoom(const char* args);
void SetModPath(const char* path);
void DoomMainLoopFunc();
void DoomRegisterAtExit(doom_atexit_func_t func, bool run_if_error);

//#ifdef __cplusplus
//}
//#endif

#endif