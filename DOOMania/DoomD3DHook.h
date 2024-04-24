#pragma once
#include "pch.h"
#include <stdint.h>
//#include "DoomD3DHook.hpp"

#ifndef DOOMD3DHOOK_H
#define DOOMD3DHOOK_H

typedef struct _doomerpal
{
	char r, g, b;
}doomerpal;

#ifdef __cplusplus
namespace DoomD3DHook
{
	bool GetFramebufferEnabled();
	void SetFramebufferEnabled(bool val);
	uint32_t GetScreenWidth();
	uint32_t GetScreenHeight();
	uint32_t* GetScreenFramebuffer();

	void Init();
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

	void DoomD3D_DumpFramebuffer(char* image, doomerpal* pal);


#ifdef __cplusplus
}
#endif

#endif
