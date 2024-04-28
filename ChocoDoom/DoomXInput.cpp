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

#include "pch.h"
#include "DoomXInput.h"

extern "C" {
#include "Chocolate DOOM/d_event.h"
#include "Chocolate DOOM/deh_str.h"
#include "Chocolate DOOM/m_misc.h"
}

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")


constexpr int controllerIndex = 0;
// ----------------------------------------------------------------------------
// Reading the action buttons
// ----------------------------------------------------------------------------
static int RE_Control_ReadIn()
{

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Get the state of the controller
	if (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS)
	{
		int res = 0;
		WORD wButtons = state.Gamepad.wButtons;
		if ((wButtons & XINPUT_GAMEPAD_Y) && (wButtons & XINPUT_GAMEPAD_B))
		{
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				res |= (1 << 6);		// swap weapon up
			}
			else if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				res |= (1 << 5);		// swap weapon down
			}
			return (res);
		}
		else
		{
			if (wButtons & XINPUT_GAMEPAD_Y)
			{
				res |= (1 << 3);		// use item
			}
			if (wButtons & XINPUT_GAMEPAD_B)
			{
				res |= (1 << 1);		// strafe
			}
		}
		if (wButtons & XINPUT_GAMEPAD_A)
		{
			res |= (1 << 0);		// fire gun
		}
		if (wButtons & XINPUT_GAMEPAD_X)
		{
			res |= (1 << 2);		// run
		}

		if (wButtons & XINPUT_GAMEPAD_START)
		{
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
static int RE_Joy_ReadIn(boolean axis)
{

	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	// Get the state of the controller
	if (XInputGetState(controllerIndex, &state) == ERROR_SUCCESS)
	{
		WORD wButtons = state.Gamepad.wButtons;
		int res = 0;
		// If we're checking the X-axis...
		if (axis == false)
		{

			//res = state.Gamepad.sThumbLX;

			if (wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				res -= 0x7FFF;
			}
			if (wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				res += 0x7FFF;
			}
		}
		// Otherwise, we're checking the Y-axis...
		else
		{
			//res = -state.Gamepad.sThumbRY;
			if (wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				res += 0x7FFF;
			}
			if (wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
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
void RE_Control()
{
	event_t ev;

	// read in input
	ev.type = ev_joystick;
	ev.data1 = RE_Control_ReadIn();
	// if the player is swapping weapons, don't listen to the input joysticks
	if (!(ev.data1 & (1 << 5)) && !(ev.data1 & (1 << 6)))
	{
		ev.data2 = RE_Joy_ReadIn(false);
		ev.data3 = RE_Joy_ReadIn(true);
	}
	else
	{
		ev.data2 = ev.data3 = 0;
	}
	ev.data4 = ev.data5 = 0;


	D_PostEvent(&ev);
}

