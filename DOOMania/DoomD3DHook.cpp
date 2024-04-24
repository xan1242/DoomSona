#include "pch.h"
#include "DoomD3DHook.h"
#include "dxhook/src/Renderer.h"
#include "dxhook/src/DirectXHook.h"
#include "dxhook/overlays/Example.h"
#include "Chocolate DOOM/i_video.h"

namespace DoomD3DHook
{
	constexpr uint32_t doomScreenSize = SCREENWIDTH * SCREENHEIGHT;
	static bool bFramebufferEnabled;
	static uint32_t FB[doomScreenSize];

	bool GetFramebufferEnabled()
	{
		return bFramebufferEnabled;
	}

	void SetFramebufferEnabled(bool val)
	{
		bFramebufferEnabled = val;
	}

	uint32_t GetScreenWidth()
	{
		return SCREENWIDTH;
	}

	uint32_t GetScreenHeight()
	{
		return SCREENHEIGHT;
	}

	uint32_t* GetScreenFramebuffer()
	{
		return FB;
	}

	void Init()
	{
		static Renderer renderer;
		static DirectXHook dxHook(&renderer);
		static Example example;

		dxHook.AddRenderCallback(&example);
		dxHook.Hook();

		example.renderer = &renderer;
	}
}

static uint32_t ConvToRGBA(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t a8 = 0xFF;

	uint32_t rgba = 0;
	rgba |= a8 | (b << 8) | (g << 16) | (r << 24);

	return rgba;
}


void DoomD3D_DumpFramebuffer(char* image, doomerpal* pal)
{
	if (!DoomD3DHook::bFramebufferEnabled)
		return;

	for (int y = 0; y < SCREENHEIGHT; y++)
	{
		for (int x = 0; x < SCREENWIDTH; x++)
		{
			unsigned char index = image[(y * SCREENWIDTH) + x];
			DoomD3DHook::FB[(y * SCREENWIDTH) + x] = ConvToRGBA(pal[index].r, pal[index].g, pal[index].b);
		}
	}
}
