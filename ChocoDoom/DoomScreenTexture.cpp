#include "pch.h"
#include "DoomScreenTexture.h"
#include "Chocolate DOOM/i_video.h"

namespace DoomScreenTexture
{
	constexpr uint32_t doomScreenSize = SCREENWIDTH * SCREENHEIGHT;
	static bool bFramebufferEnabled;
	static uint32_t FB[doomScreenSize];

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
}

static uint32_t ConvToRGBA(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t a8 = 0xFF;

	uint32_t rgba = 0;
	rgba |= a8 | (b << 8) | (g << 16) | (r << 24);

	return rgba;
}


void DoomScreenTexture_DumpFramebuffer(char* image, doomerpal* pal)
{
	for (int y = 0; y < SCREENHEIGHT; y++)
	{
		for (int x = 0; x < SCREENWIDTH; x++)
		{
			unsigned char index = image[(y * SCREENWIDTH) + x];
			DoomScreenTexture::FB[(y * SCREENWIDTH) + x] = ConvToRGBA(pal[index].r, pal[index].g, pal[index].b);
		}
	}
}
