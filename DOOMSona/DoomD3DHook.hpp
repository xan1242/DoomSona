#pragma once
#include "framework.h"
//#include <stdint.h>
//#include "DoomD3DHook.hpp"

#ifndef DOOMD3DHOOK_HPP
#define DOOMD3DHOOK_HPP

namespace DoomD3DHook
{
	bool GetFramebufferEnabled();
	void SetFramebufferEnabled(bool val);

	void Init();
}

#endif
