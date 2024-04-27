#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "DoomWin32.h"
#include "Chocolate DOOM/doomkeys.h"

HWND wndHandle;

#include <map>
#include <vector>
std::map<int, int> sVkeyDoomMap =
{
	{VK_RIGHT, KEY_RIGHTARROW},
	{VK_LEFT, KEY_LEFTARROW},
	{VK_UP, KEY_UPARROW},
	{VK_DOWN, KEY_DOWNARROW},
	{VK_ESCAPE, KEY_ESCAPE},
	{VK_RETURN, KEY_ENTER},
	{VK_TAB, KEY_TAB},
	{VK_F1, KEY_F1},
	{VK_F2, KEY_F2},
	{VK_F3, KEY_F3},
	{VK_F4, KEY_F4},
	{VK_F5, KEY_F5},
	{VK_F6, KEY_F6},
	{VK_F7, KEY_F7},
	{VK_F8, KEY_F8},
	{VK_F9, KEY_F9},
	{VK_F10, KEY_F10},
	{VK_F11, KEY_F11},
	{VK_F12, KEY_F12},
	{VK_BACK, KEY_BACKSPACE},
	{VK_RSHIFT, KEY_RSHIFT},
	{VK_LSHIFT, KEY_RSHIFT},
	{VK_SHIFT, KEY_RSHIFT},
	{VK_LCONTROL, KEY_RCTRL},
	{VK_RCONTROL, KEY_RCTRL},
	{VK_MENU, KEY_LALT},
	{VK_RMENU, KEY_RALT},
	{VK_CAPITAL, KEY_CAPSLOCK},
	{VK_NUMLOCK, KEY_NUMLOCK},
	{VK_SCROLL, KEY_SCRLCK},
	{VK_PRINT, KEY_PRTSCR},
	{VK_HOME, KEY_HOME},
	{VK_END, KEY_END},
	{VK_PRIOR, KEY_PGUP},
	{VK_NEXT, KEY_PGDN},
	{VK_INSERT, KEY_INS},
	{VK_DELETE, KEY_DEL},
	{VK_DIVIDE, KEYP_DIVIDE},
	{VK_ADD, KEYP_PLUS},
	{VK_SUBTRACT, KEY_MINUS},
	{VK_MULTIPLY, KEYP_MULTIPLY},
	{VK_DELETE, KEY_DEL},
};

std::vector<int> sDoomKeyCodeList =
{
	KEY_RIGHTARROW,
	KEY_LEFTARROW,
	KEY_UPARROW,
	KEY_DOWNARROW,
	KEY_ESCAPE,
	KEY_ENTER,
	KEY_TAB,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_BACKSPACE,
	KEY_PAUSE,
	KEY_EQUALS,
	KEY_MINUS,
	KEY_RSHIFT,
	KEY_RCTRL,
	KEY_RALT,
	KEY_LALT,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_SCRLCK,
	KEY_PRTSCR,
	KEY_HOME,
	KEY_END,
	KEY_PGUP,
	KEY_PGDN,
	KEY_INS,
	KEY_DEL,
	KEYP_MULTIPLY,
	KEYP_DIVIDE,
	KEYP_PLUS
};

void __declspec(dllexport) DoomWin32_SetHWND(HWND hWnd)
{
	wndHandle = hWnd;
}

HWND __declspec(dllexport) DoomWin32_GetHWND()
{
	return wndHandle;
}

static bool bIsMouseVkey(int vkey)
{
	switch (vkey)
	{
	case VK_LBUTTON:
	case VK_RBUTTON:
	case VK_MBUTTON:
	case VK_XBUTTON1:
	case VK_XBUTTON2:
		return true;
	default:
		return false;
	}
}

int TranslateWin32Key(int vkey)
{
	if (bIsMouseVkey(vkey))
		return 0;

	auto it = sVkeyDoomMap.find(vkey);
	if (it == sVkeyDoomMap.end())
	{
		if (std::find(sDoomKeyCodeList.begin(), sDoomKeyCodeList.end(), vkey) != sDoomKeyCodeList.end())
			return 0;

		if (isalpha(vkey))
			return tolower(vkey);
		return vkey;
	}

	return it->second;
}
