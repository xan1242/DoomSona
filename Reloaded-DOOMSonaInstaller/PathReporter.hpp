#pragma once

#ifndef PATHREPORTER_HPP
#define PATHREPORTER_HPP

#define RLD_DOOMSONA_API extern "C" __declspec(dllexport) 

namespace PathReporter
{
	void Init();
}

RLD_DOOMSONA_API size_t PathReporter_GetModPathLength();
RLD_DOOMSONA_API const char* PathReporter_GetModPathA(char* outStr);
RLD_DOOMSONA_API const char16_t* PathReporter_GetModPathW(char16_t* outStr);

#endif
