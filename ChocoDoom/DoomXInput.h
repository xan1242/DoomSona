#pragma once
#include "pch.h"

#ifndef DOOMXINPUT_H
#define DOOMXINPUT_H

#ifdef __cplusplus
extern "C" {
#endif
	int XInput_GetMovementYAxis();
	int XInput_GetTurnAxis();
	int XInput_GetMovementStrafeAxis();
	float XInput_AxisToPercentage(SHORT axis);
	float XInput_Lerp(float a, float b, float t);
	void RE_Control();

#ifdef __cplusplus
}
#endif

#endif