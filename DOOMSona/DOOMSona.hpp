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
		DPSP_FILEEXISTS,
		DPSP_FILESIZE,
		DPSP_APPLYDOOMARGS,
		DPSP_KILLS,
		DPSP_SECRETS,

		DPSP_COUNT
	};

	enum DoomIWads
	{
		IWAD_UNK = -1,
		IWAD_DEFAULT,
		IWAD_DOOMSHAREWARE,
		IWAD_DOOM,
		IWAD_DOOM2,
		IWAD_FREEDOOM1,
		IWAD_FREEDOOM2,

		IWAD_COUNT
	};

	void Init();
}

#endif
