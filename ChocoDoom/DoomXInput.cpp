#define NOMINMAX
#include "pch.h"
#include "DoomXInput.h"

extern "C" {
#include "Chocolate DOOM/d_event.h"
#include "Chocolate DOOM/deh_str.h"
#include "Chocolate DOOM/m_misc.h"
}

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#include <limits>
#include <cstdlib>
#include <cmath>

constexpr int controllerIndex = 0;

constexpr SHORT INPUT_DEADZONE_LS = (0.24f * std::numeric_limits<SHORT>::max());
constexpr SHORT INPUT_DEADZONE_RS = (0.24f * std::numeric_limits<SHORT>::max());
constexpr SHORT TRIGGER_ACTIVATION_THRESHOLD = (0.12f * std::numeric_limits<BYTE>::max());  // 12% for analog triggers digital activation

static int RE_Control_ReadIn()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(controllerIndex, &state) != ERROR_SUCCESS)
		return 0;

	int res = 0;
	WORD wButtons = state.Gamepad.wButtons;

	if (wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		res |= (1 << 6);		// swap weapon up
	}
	else if (wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		res |= (1 << 5);		// swap weapon down
	}

	if ((wButtons & XINPUT_GAMEPAD_A) || (wButtons & XINPUT_GAMEPAD_RIGHT_THUMB))
	{
		res |= (1 << 3);		// use item
	}
	if (wButtons & XINPUT_GAMEPAD_X)
	{
		res |= (1 << 1);		// strafe
	}

	if (state.Gamepad.bRightTrigger > TRIGGER_ACTIVATION_THRESHOLD)
	{
		res |= (1 << 0);		// fire gun
	}

	if (state.Gamepad.bLeftTrigger > TRIGGER_ACTIVATION_THRESHOLD)
	{
		res |= (1 << 2);		// run
	}

	if (wButtons & XINPUT_GAMEPAD_START)
	{
		res |= (1 << 4);		// activate menu
	}

	if (wButtons & XINPUT_GAMEPAD_BACK)
	{
		res |= (1 << 7);		// activate automap
	}

	return res;
}

static SHORT calculateAxisValue(short rawValue, short deadzone)
{
	// Calculate the deadzone-adjusted value
	if (std::abs(rawValue) < deadzone)
	{
		return 0;  // Value within deadzone, return 0
	}
	else
	{
		// Calculate the adjusted value outside the deadzone
		SHORT sign = (rawValue > 0) ? 1 : -1;
		float ratio = (std::abs(rawValue) - deadzone) / ((float)std::numeric_limits<SHORT>::max() - deadzone);
		SHORT adjustedValue = static_cast<short>(ratio * (float)std::numeric_limits<SHORT>::max()) * sign;
		return adjustedValue;
	}
}

int XInput_GetTurnAxis()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(controllerIndex, &state) != ERROR_SUCCESS)
		return 0;

	return calculateAxisValue(state.Gamepad.sThumbRX, INPUT_DEADZONE_RS);
}

int XInput_GetMovementYAxis()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(controllerIndex, &state) != ERROR_SUCCESS)
		return 0;

	return calculateAxisValue(-state.Gamepad.sThumbLY, INPUT_DEADZONE_LS);
}

int XInput_GetMovementStrafeAxis()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(controllerIndex, &state) != ERROR_SUCCESS)
		return 0;

	return calculateAxisValue(state.Gamepad.sThumbLX, INPUT_DEADZONE_LS);
}

float XInput_AxisToPercentage(SHORT axis)
{
	return (float)axis / std::numeric_limits<SHORT>::max();
}

float XInput_Lerp(float a, float b, float t)
{
	return std::lerp(a, b, t);
}

void RE_Control()
{
	event_t ev;

	// read in input
	ev.type = ev_joystick;
	ev.data1 = RE_Control_ReadIn();
	ev.data2 = 0;
	ev.data3 = 0;
	//ev.data4 = 0;
	ev.data4 = XInput_Lerp(0.0f, 40.0f, XInput_AxisToPercentage(XInput_GetMovementStrafeAxis()));
	ev.data5 = 0;

	//ev.data2 = XInput_GetTurnAxis();
	//ev.data3 = XInput_GetMovementYAxis();
	//ev.data4 = XInput_GetMovementStrafeAxis();
	//ev.data5 = 0;

	D_PostEvent(&ev);
}

