#pragma once

#include "../src/IRenderCallback.h"
#include "../src/OverlayFramework.h"
#include "../src/Renderer.h"

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

	Renderer* renderer;

private:
	OF::Box* box;
	int doomtex = 0;
};

#endif