//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
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
// DESCRIPTION:
//     SDL implementation of system-specific input interface.
//


//#include "SDL.h"
//#include "SDL_keycode.h"

#include "doomkeys.h"
#include "doomtype.h"
#include "d_event.h"
#include "i_input.h"
#include "m_argv.h"
#include "m_config.h"
#include "g_game.h"

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>
#include "../DoomWin32.h"
#include "../DoomXInput.h"

static const int scancode_translate_table[] = SCANCODE_TO_KEYS_ARRAY;

// Lookup table for mapping ASCII characters to their equivalent when
// shift is pressed on a US layout keyboard. This is the original table
// as found in the Doom sources, comments and all.
static const char shiftxform[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, ' ', '!', '"', '#', '$', '%', '&',
    '"', // shift-'
    '(', ')', '*', '+',
    '<', // shift-,
    '_', // shift--
    '>', // shift-.
    '?', // shift-/
    ')', // shift-0
    '!', // shift-1
    '@', // shift-2
    '#', // shift-3
    '$', // shift-4
    '%', // shift-5
    '^', // shift-6
    '&', // shift-7
    '*', // shift-8
    '(', // shift-9
    ':',
    ':', // shift-;
    '<',
    '+', // shift-=
    '>', '?', '@',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '[', // shift-[
    '!', // shift-backslash - OH MY GOD DOES WATCOM SUCK
    ']', // shift-]
    '"', '_',
    '\'', // shift-`
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '{', '|', '}', '~', 127
};

// If true, I_StartTextInput() has been called, and we are populating
// the data3 field of ev_keydown events.
static boolean text_input_enabled = true;

// Bit mask of mouse button state.
//static unsigned int mouse_button_state = 0;

// Disallow mouse and joystick movement to cause forward/backward
// motion.  Specified with the '-novert' command line parameter.
// This is an int to allow saving to config file
int novert = 0;

// If true, keyboard mapping is ignored, like in Vanilla Doom.
// The sensible thing to do is to disable this if you have a non-US
// keyboard.

int vanilla_keyboard_mapping = true;

// Mouse acceleration
//
// This emulates some of the behavior of DOS mouse drivers by increasing
// the speed when the mouse is moved fast.
//
// The mouse input values are input directly to the game, but when
// the values exceed the value of mouse_threshold, they are multiplied
// by mouse_acceleration to increase the speed.
float mouse_acceleration = 2.0;
int mouse_threshold = 10;

#ifdef _MSC_VER
__declspec(noinline)
#endif
static int GetTypedChar(int vk)
{
    int result = TranslateWin32Key(vk);


    // If shift is held down, apply the original uppercase
    // translation table used under DOS.
    if ((GetAsyncKeyState(VK_SHIFT) >> 15)
        && result >= 0 && result < arrlen(shiftxform))
    {
        result = shiftxform[result];
    }

    return result;
}


boolean prevStates[256];

// #TODO: figure out why this breaks kb input - 2x inputs on cursors, weird behavior in general with GetAsyncKeyState

#ifdef _MSC_VER
__declspec(noinline)
#endif
void I_HandleKeyboardEvent()
{
    // XXX: passing pointers to event for access after this function
    // has terminated is undefined behaviour
    event_t event;

    for (int i = 1; i < 256; ++i)
    {
        boolean bState = (GetAsyncKeyState(i) >> 15) & 1;

        // Check for key down event
        if (bState)
        {
            if (!(prevStates[i]))
            {
                event.type = ev_keydown;
                event.data1 = TranslateWin32Key(i);
                event.data2 = TranslateWin32Key(i);
                event.data3 = GetTypedChar(i);

                //event.data1 = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);
                //event.data2 = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);
                //event.data3 = MapVirtualKeyA(i, MAPVK_VK_TO_CHAR);

                if (event.data1 != 0)
                {
                    D_PostEvent(&event);
                }
            }
        }
        // Check for key up event
        else
        {
            if (prevStates[i])
            {
                event.type = ev_keyup;
                event.data1 = TranslateWin32Key(i);
                //event.data1 = MapVirtualKeyA(i, MAPVK_VK_TO_VSC);

                // data2/data3 are initialized to zero for ev_keyup.
                // For ev_keydown it's the shifted Unicode character
                // that was typed, but if something wants to detect
                // key releases it should do so based on data1
                // (key ID), not the printable char.

                event.data2 = 0;
                event.data3 = 0;

                if (event.data1 != 0)
                {
                    D_PostEvent(&event);
                }
            }
        }

        prevStates[i] = bState;
    }
}

static void Win32_GetWindowCenter(POINT* outPoint)
{
    HWND hWnd = DoomWin32_GetHWND();
    RECT windowRect;
    GetWindowRect(hWnd, &windowRect);

    outPoint->x = ((windowRect.right - windowRect.left) / 2) + windowRect.left;
    outPoint->y = ((windowRect.bottom - windowRect.top) / 2) + windowRect.top;
}

static void Win32_CenterMouse()
{
    POINT centerPos;
    Win32_GetWindowCenter(&centerPos);

    SetCursorPos(centerPos.x, centerPos.y);
}

static int Win32_GetRelativeMouseState(int* x, int* y)
{
    POINT currentPos;
    POINT centerPos;
    Win32_GetWindowCenter(&centerPos);
    GetCursorPos(&currentPos);

    int deltaX = currentPos.x - centerPos.x;
    int deltaY = currentPos.y - centerPos.y;

    *x = deltaX;
    *y = deltaY;

    return 0;
}

static int AccelerateMouse(int val)
{
    if (val < 0)
        return -AccelerateMouse(-val);

    if (val > mouse_threshold)
    {
        return (int)((val - mouse_threshold) * mouse_acceleration + mouse_threshold);
    }
    else
    {
        return val;
    }
}

// #TODO: figure out why this breaks input - weird behavior in general with GetAsyncKeyState

//#ifdef _MSC_VER
//__declspec(noinline)
//#endif
//static void UpdateMouseButtonState()
//{
//    static event_t event;
//
//    mouse_button_state = 0;
//
//    if ((GetAsyncKeyState(VK_LBUTTON) >> 15) & 1)
//    {
//        mouse_button_state |= (1 << 0);
//    }
//
//    if ((GetAsyncKeyState(VK_RBUTTON) >> 15) & 1)
//    {
//        mouse_button_state |= (1 << 1);
//    }
//
//    if ((GetAsyncKeyState(VK_MBUTTON) >> 15) & 1)
//    {
//        mouse_button_state |= (1 << 2);
//    }
//
//    // Post an event with the new button state.
//
//    if (mouse_button_state)
//    {
//        event.type = ev_mouse;
//        event.data1 = mouse_button_state;
//        event.data2 = 0;
//        event.data3 = 0;
//        D_PostEvent(&event);
//    }
//}

//
// Read the change in mouse state to generate mouse motion events
//
// This is to combine all mouse movement for a tic into one mouse
// motion event.
#ifdef _MSC_VER
__declspec(noinline)
#endif
void I_ReadMouse(void)
{
    int x, y;
    event_t ev;

    //UpdateMouseButtonState();

    int mousebuttons = 0;

    if ((GetAsyncKeyState(VK_LBUTTON) >> 15) & 1)
    {
        mousebuttons |= (1 << 0);
    }

    if ((GetAsyncKeyState(VK_RBUTTON) >> 15) & 1)
    {
        mousebuttons |= (1 << 1);
    }

    if ((GetAsyncKeyState(VK_MBUTTON) >> 15) & 1)
    {
        mousebuttons |= (1 << 2);
    }

    Win32_GetRelativeMouseState(&x, &y);
    Win32_CenterMouse();

    ev.data1 = mousebuttons;

    //if (x != 0 || y != 0)
    {
        ev.type = ev_mouse;
        ev.data2 = AccelerateMouse(x);

        if (!novert)
        {
            ev.data3 = -AccelerateMouse(y);
        }
        else
        {
            ev.data3 = 0;
        }

        // HACK: add in controller input here because joy inputs are SUPER pepeg
        ev.data2 += XInput_Lerp(0.0f, 160.0f, XInput_AxisToPercentage(XInput_GetTurnAxis()));
        ev.data3 += XInput_Lerp(0.0f, 50.0f, XInput_AxisToPercentage(-XInput_GetMovementYAxis()));


        // XXX: undefined behaviour since event is scoped to
        // this function
        D_PostEvent(&ev);
    }
}

void I_StartTextInput(int x1, int y1, int x2, int y2)
{
	// == COME BACK ==
}

void I_StopTextInput(void)
{
	// == COME BACK ==
}

// Bind all variables controlling input options.
void I_BindInputVariables(void)
{
    M_BindFloatVariable("mouse_acceleration",      &mouse_acceleration);
    M_BindIntVariable("mouse_threshold",           &mouse_threshold);
    M_BindIntVariable("vanilla_keyboard_mapping",  &vanilla_keyboard_mapping);
    M_BindIntVariable("novert",                    &novert);
}
