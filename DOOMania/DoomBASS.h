#pragma once
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#include "includes/bass/c/bass.h"
#include <wtypes.h>
//#include "Chocolate DOOM/doomtype.h"

#include "Chocolate DOOM/i_sound.h"


#ifndef DOOMBASS_H
#define DOOMBASS_H

#ifdef __cplusplus
extern "C" {
#endif
	boolean DoomBASS_GetBassInited();
	void DoomBASS_SetBassInited(boolean val);
	int DoomBASS_Shutdown();
	int DoomBASS_Init(int samplerate);
	extern const music_module_t DoomBASS_module;
#ifdef __cplusplus
}
#endif

#endif