#pragma once
#include "pch.h"
#include <stdint.h>
//#include "DoomD3DHook.hpp"

#ifndef DOOMSCREENTEXTURE_H
#define DOOMSCREENTEXTURE_H

typedef struct _doomerpal
{
	char r, g, b;
}doomerpal;

#ifdef __cplusplus
namespace DoomScreenTexture
{
	uint32_t GetScreenWidth();
	uint32_t GetScreenHeight();
	uint32_t* GetScreenFramebuffer();
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

	void DoomScreenTexture_DumpFramebuffer(char* image, doomerpal* pal);

#ifdef __cplusplus
}
#endif

#endif
