#include "pch.h"
#include <vector>
#include <utility>

#include "DoomLevelCompletionTracker.h"

namespace DoomLevelCompletionTracker
{
	struct TrackerInfo
	{
		int episode;
		int map;
		int skill;

		// Define spaceship operator <=>
		auto operator<=>(const TrackerInfo&) const = default; // Use default comparison

		// Optional: Define equality operator
		bool operator==(const TrackerInfo& other) const
		{
			return episode == other.episode && map == other.map && skill == other.skill;
		}
	};

	static std::vector<TrackerInfo> sCompletedMaps;

	static void Add(int episode, int map, int skill)
	{
		int completeCount = 0;
		TrackerInfo i = { episode, map, skill };

		if (std::find(sCompletedMaps.begin(), sCompletedMaps.end(), i) == sCompletedMaps.end())
		{
			sCompletedMaps.push_back(i);
		}
	}

	static int GetUniqueMapCount()
	{
		int lastEp = -1;
		int lastMap = -1;
		int count = 0;
		for (const auto& i : sCompletedMaps)
		{
			if ((i.episode != lastEp) || (i.map != lastMap))
			{
				count++;
				lastEp = i.episode;
				lastMap = i.map;
			}
		}

		return count;
	}

	static int GetUniqueMapCount(int skill)
	{
		int lastEp = -1;
		int lastMap = -1;
		int count = 0;
		for (const auto& i : sCompletedMaps)
		{
			if (skill == i.skill)
			{
				if ((i.episode != lastEp) || (i.map != lastMap))
				{
					count++;
					lastEp = i.episode;
					lastMap = i.map;
				}
			}
		}

		return count;
	}
}

void DoomLevelCompletionTracker_Add(int episode, int map, int skill)
{
	return DoomLevelCompletionTracker::Add(episode, map, skill);
}

int DoomLevelCompletionTracker_GetUniqueMapCount()
{
	return DoomLevelCompletionTracker::GetUniqueMapCount();
}

int DoomLevelCompletionTracker_GetUniqueMapCountBySkill(int skill)
{
	return DoomLevelCompletionTracker::GetUniqueMapCount(skill);
}
