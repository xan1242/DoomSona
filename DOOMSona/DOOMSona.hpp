#pragma once
#include "framework.h"

#ifndef DOOMSONA_HPP
#define DOOMSONA_HPP

#ifdef _DEBUG
#define STRCONST const
#else
#define STRCONST constexpr
#endif

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
		DPSP_COMPLETEDLEVELS,
		DPSP_COMPLETEDLEVELSBYSKILL,
		DPSP_FINISHEDSHAREWARE,

		DPSP_COUNT
	};

	enum skill_t
	{
		sk_noitems = -1,        // the "-skill 0" hack
		sk_baby = 0,
		sk_easy,
		sk_medium,
		sk_hard,
		sk_nightmare,
		sk_count
	};

	void Init();
}

#endif
