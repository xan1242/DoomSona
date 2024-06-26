#pragma once

#ifdef _DEBUG

#include <string>
#include <cstdarg>

class Logger
{
private:
	char DebugPrintBuffer[4096];
public:
	Logger(const char* prefix)
	{
		printPrefix = prefix;

		FILE* logFile = GetLogFile();
		if (logFile == nullptr)
		{
			fopen_s(&logFile, "hook_log.txt", "w");
			GetLogFile(logFile);
		}
	}

	void Log(std::string msg, ...)
	{
		va_list args;
		va_start(args, msg);
		vprintf(std::string(printPrefix + " > " + msg + "\n").c_str(), args);
		if (GetLogFile() != nullptr)
		{
			vfprintf(GetLogFile(), std::string(printPrefix + " > " + msg + "\n").c_str(), args);
			vsprintf(DebugPrintBuffer, std::string(printPrefix + " > " + msg + "\n").c_str(), args);
			fflush(GetLogFile());
			OutputDebugStringA(DebugPrintBuffer);
		}
		va_end(args);
	}

private:
	std::string printPrefix = "";

	static FILE* GetLogFile(FILE* newLogFile = nullptr)
	{
		static FILE* logFile = nullptr;
		if (newLogFile != nullptr)
		{
			logFile = newLogFile;
		}
		return logFile;
	}
};

#endif