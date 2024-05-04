//
// Reports the parent path of this module
//

#include "PathReporter.hpp"
#include "ModPath.hpp"
#include <filesystem>

namespace PathReporter
{
	std::filesystem::path pathThisMod;

	void Init()
	{
		pathThisMod = ModPath::GetThisModulePath<std::filesystem::path>();
	}
}

RLD_DOOMSONA_API size_t PathReporter_GetModPathLength()
{
	return PathReporter::pathThisMod.wstring().size();
}

RLD_DOOMSONA_API const char* PathReporter_GetModPathA(char* outStr)
{
	strncpy(outStr, PathReporter::pathThisMod.string().c_str(), PathReporter::pathThisMod.string().size());
	return outStr;
}

RLD_DOOMSONA_API const char16_t* PathReporter_GetModPathW(char16_t* outStr)
{
	//strncpy(outStr, PathReporter::pathThisMod.string().c_str(), PathReporter::pathThisMod.string().size());
	//wcsncpy(outStr, PathReporter::pathThisMod.wstring().c_str(), PathReporter::pathThisMod.wstring().size());
	memcpy(outStr, PathReporter::pathThisMod.wstring().c_str(), PathReporter::pathThisMod.wstring().size() * sizeof(char16_t));
	return outStr;
}
