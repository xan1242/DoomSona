#pragma once

#ifndef DOOMLEVELCOMPLETIONTRACKER_H
#define DOOMLEVELCOMPLETIONTRACKER_H

#ifdef __cplusplus
extern "C" {
#endif

	void DoomLevelCompletionTracker_Add(int episode, int map, int skill);
	int DoomLevelCompletionTracker_GetUniqueMapCount();
	int DoomLevelCompletionTracker_GetUniqueMapCountBySkill(int skill);

#ifdef __cplusplus
}
#endif

#endif // DOOMLEVELCOMPLETIONTRACKER_H
