#pragma once

#include <fstream>

// Very simple logging for the purpose of debugging only.

namespace Logging
{
	extern std::ofstream LOG;
	void LogFormat(char * fmt, ...);
}

#define Log() LOG

