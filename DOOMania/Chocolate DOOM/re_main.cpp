//
// Copyright(C) 2020 TheStoneBanana
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:  Retro Engine interface stuff
//
#include "../pch.h"
#include "../DDS.h"
#include <map>
#include <stdexcept>
#include <string>
#include <iostream>

extern "C" {
#include "d_event.h"
#include "deh_str.h"
#include "m_misc.h"

#include "sounds.h"
}

#include "re_main.h"

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

//#include <windows.h>
// #include <d3d11.h>
// #pragma comment(lib, "d3d11.lib")

// ----------------------------------------------------------------------------
// Leaving the DOOM engine state and returning to Mania
// ----------------------------------------------------------------------------
void RE_Doom_Exit() {
	// SonicMania::ChangeScene(SonicMania::Scene_MainMenu);
	// SonicMania::Engine_ClearSoundFXData();
}



constexpr uint32_t doomScreenSizeX = 424;
constexpr uint32_t doomScreenSizeY = 240;
constexpr uint32_t doomScreenSize = doomScreenSizeX * doomScreenSizeY;
uint32_t tempFB[doomScreenSize];

bool bTempBool = false;

uint32_t ConvToARGB(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t a8 = 0xFF;

	uint32_t rgba = 0;
	rgba |= b | (g << 8) | (r << 16) | (a8 << 24);

	return rgba;
}

uint32_t ConvToRGBA(uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t a8 = 0xFF;

	uint32_t rgba = 0;
	rgba |= a8 | (b << 8) | (g << 16) | (r << 24);

	return rgba;
}

void WriteImage(std::string filename, void* buffer, unsigned int width, unsigned int height)
{
	// create dds
	struct DirectX::DDS_HEADER ddshs = { 0 };
	struct DirectX::DDS_PIXELFORMAT ddspfs = { 0 };
	ddshs.dwSize = 124;
	ddshs.dwFlags = 0x21007;

	ddshs.dwWidth = width;
	ddshs.dwHeight = height;

	ddshs.dwMipMapCount = 0;
	ddspfs.dwSize = 32;
	ddspfs.dwFlags = 0x41;
	ddspfs.dwRGBBitCount = 0x20;
	ddspfs.dwRBitMask = 0xFF0000;
	ddspfs.dwGBitMask = 0xFF00;
	ddspfs.dwBBitMask = 0xFF;
	ddspfs.dwABitMask = 0xFF000000;
	ddshs.dwCaps = 0x40100A;
	ddshs.ddspf = ddspfs;
	constexpr unsigned int DDSMagic = 0x20534444;

	size_t writeSize = (width * height) * sizeof(uint32_t);


	std::cout << "Writing: " << filename << '\n';

	FILE* fout = fopen(filename.c_str(), "wb");
	if (!fout)
	{
		std::cout << "ERROR: couldn't open file for writing: " << filename << '\n';
		perror("ERROR");
		return;
	}

	fwrite(&DDSMagic, 4, 1, fout);
	fwrite(&ddshs, sizeof(ddshs), 1, fout);
	fwrite(buffer, sizeof(uint8_t), writeSize, fout);

	fclose(fout);
}

extern "C" __declspec(dllexport) uint32_t* GetScreenBuffer()
{
	//printf("Sending buffer...\n");
	return tempFB;
}

extern "C" __declspec(dllexport) uint32_t GetScreenX()
{
	return doomScreenSizeX;
}

extern "C" __declspec(dllexport) uint32_t GetScreenY()
{
	return doomScreenSizeY;
}

// void UpdateD3D11Buffer(ID3D11DeviceContext* context, ID3D11Resource* textureSRV)
// {
// 	D3D11_MAPPED_SUBRESOURCE mappedResource;
// 	HRESULT hr = context->Map(textureSRV, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
// 
// 	if (SUCCEEDED(hr))
// 	{
// 		memcpy(mappedResource.pData, tempFB, doomScreenSize * sizeof(uint32_t));
// 		context->Unmap(textureSRV, 0);
// 	}
// }

// ----------------------------------------------------------------------------
// Dumping out DOOM's framebuffer to Mania's framebuffer
// ----------------------------------------------------------------------------
void RE_Framebuffer_Dump(char* image, doomerpal* pal) {


		for (int y = 0; y < doomScreenSizeY; y++) {
			for (int x = 0; x < doomScreenSizeX; x++) {
				unsigned char index = image[(y * doomScreenSizeX) + x];
				tempFB[(y * doomScreenSizeX) + x] = ConvToRGBA(pal[index].r, pal[index].g, pal[index].b);
			}
		}

		if (!bTempBool)
		{
			WriteImage("test.dds", tempFB, doomScreenSizeX, doomScreenSizeY);
			bTempBool = true;
		}
}
constexpr int controllerIndex = 0;
// ----------------------------------------------------------------------------
// Reading the action buttons
// ----------------------------------------------------------------------------
int RE_Control_ReadIn() {

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Get the state of the controller
	if (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS) 
	{
		int res = 0;
		WORD wButtons = state.Gamepad.wButtons;
		if ((wButtons & XINPUT_GAMEPAD_Y) && (wButtons & XINPUT_GAMEPAD_B)) {
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				res |= (1 << 6);		// swap weapon up
			}
			else if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				res |= (1 << 5);		// swap weapon down
			}
			return (res);
		}
		else {
			if (wButtons & XINPUT_GAMEPAD_Y) {
				res |= (1 << 3);		// use item
			}
			if (wButtons & XINPUT_GAMEPAD_B) {
				res |= (1 << 1);		// strafe
			}
		}
		if (wButtons & XINPUT_GAMEPAD_A) {
			res |= (1 << 0);		// fire gun
		}
		if (wButtons & XINPUT_GAMEPAD_X) {
			res |= (1 << 2);		// run
		}
		
		if (wButtons & XINPUT_GAMEPAD_START) {
			res |= (1 << 4);		// activate menu
		}
		
		return res;
	}
	return 0;

	//return 0;
}



// ----------------------------------------------------------------------------
// Reading the D-Pad
// ----------------------------------------------------------------------------
int RE_Joy_ReadIn(boolean axis) {

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Get the state of the controller
	if (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS) {
		WORD wButtons = state.Gamepad.wButtons;
		int res = 0;
		// If we're checking the X-axis...
		if (axis == false) {

			//res = state.Gamepad.sThumbLX;

			if (wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				res -= 0x7FFF;
			}
			if (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				res += 0x7FFF;
			}
		}
		// Otherwise, we're checking the Y-axis...
		else {
			//res = -state.Gamepad.sThumbRY;
			if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				res += 0x7FFF;
			}
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				res -= 0x7FFF;
			}
		}
		return res;
	}

	return 0;
}

// ----------------------------------------------------------------------------
// Constructing a joystick input event to post to DOOM
// ----------------------------------------------------------------------------
void RE_Control() {
	event_t ev;
	
	// read in input
	ev.type = ev_joystick;
	ev.data1 = RE_Control_ReadIn();
	// if the player is swapping weapons, don't listen to the input joysticks
	if (!(ev.data1 & (1 << 5)) && !(ev.data1 & (1 << 6))) {
		ev.data2 = RE_Joy_ReadIn(false);
		ev.data3 = RE_Joy_ReadIn(true);
	}
	else {
		ev.data2 = ev.data3 = 0;
	}
	ev.data4 = ev.data5 = 0;
	
	
	D_PostEvent(&ev);
}

typedef struct {
	const char* file;
	unsigned int loop;
} maniamusicinfo;

const std::map<std::string, maniamusicinfo> musicmap = {
	{ "introa", {"d_intro.ogg", 0} },
	// Episode 1
	{ "e1m1", {"d_e1m1.ogg", 94131} },
	{ "e1m2", {"d_e1m2.ogg", 142653} },
	{ "e1m3", {"d_e1m3.ogg", 216905} },
	{ "e1m4", {"d_e1m4.ogg", 101156} },
	{ "e1m5", {"d_e1m5.ogg", 347759} },
	{ "e1m6", {"d_e1m6.ogg", 129897} },
	{ "e1m7", {"d_e1m7.ogg", 297731} },
	{ "e1m8", {"d_e1m8.ogg", 346645} },
	{ "e1m9", {"d_e1m9.ogg", 135375} },
	// Episode 2
	{ "e2m1", {"d_e2m1.ogg", 122076} },
	{ "e2m2", {"d_e2m2.ogg", 86574} },
	{ "e2m3", {"d_e2m3.ogg", 150340} },
	{ "e2m4", {"d_e2m4.ogg", 176063} },
	{ "e2m5", {"d_e2m5.ogg", 297731} },
	{ "e2m6", {"d_e2m6.ogg", 316010} },
	{ "e2m7", {"d_e2m7.ogg", 408705} },
	{ "e2m8", {"d_e2m8.ogg", 350665} },
	{ "e2m9", {"d_e2m9.ogg", 224927} },
	// Episode 3
	{ "e3m1", {"d_e3m1.ogg", 224927} },
	{ "e3m2", {"d_e3m2.ogg", 114372} },
	{ "e3m3", {"d_e3m3.ogg", 186970} },
	{ "e3m4", {"d_e3m4.ogg", 346645} },
	{ "e3m5", {"d_e3m5.ogg", 297731} },
	{ "e3m6", {"d_e3m6.ogg", 129897} },
	{ "e3m7", {"d_e3m7.ogg", 408705} },
	{ "e3m8", {"d_e3m8.ogg", 343273} },
	{ "e3m9", {"d_e3m9.ogg", 135375} },
	// Misc.
	{ "inter", {"d_inter.ogg", 150340} },
	{ "bunny", {"d_bunny.ogg", 0} }
};

// Map music pack OGGs to internal music IDs and play them in the Retro Engine
void RE_Music_Play(musicinfo_t* song) {
	// maniamusicinfo curSong;
	// try {
	// 	curSong = musicmap.at(song->name);
	// }
	// catch (const std::out_of_range& oor) {
	// 	printf("RE_Music_Play: Match for %.6s not found\n", song->name);
	// 	return;	// if the requested song doesn't have a matching music pack entry, ignore it
	// }
	// 
	// SonicMania::PlayMusic(curSong.file, 0, 0, curSong.loop, true);
}