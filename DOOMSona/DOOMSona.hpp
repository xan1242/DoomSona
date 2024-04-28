#pragma once
#include "framework.h"

#ifndef DOOMSONA_HPP
#define DOOMSONA_HPP

namespace DOOMSona
{
	enum DoomPersonaScriptParams
	{
		DPSP_UNK = -1,
		DPSP_ISGAMERUNNING,
		DPSP_CURRBGM,
		DPSP_KILLS,
		DPSP_SECRETS,

		DPSP_COUNT
	};

	void Init();
}

#endif
