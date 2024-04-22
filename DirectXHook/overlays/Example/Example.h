#pragma once

#include "IRenderCallback.h"
#include "OverlayFramework.h"

#ifndef EXAMPLE_MOD
#define EXAMPLE_MOD

class Example : public IRenderCallback
{
public:
	void Setup();
	void Render();

	//void* (__cdecl* GetScreenBuffer)();
	//uint32_t(__cdecl* GetScreenX)();
	//uint32_t(__cdecl* GetScreenY)();
	void UpdateD3D11Buffer();

private:
	OF::Box* box;
	int doomtex = 0;
};

#endif