#include "pch_engine.h"

#include "HeliosEngine/Utils/Path.h"


// platform headers
#if defined HE_SYSTEM_WINDOS
	#include <Windows.h>
	#include <stdlib.h>
#elif defined HE_SYSTEM_LINUX
	#include <string>
	#include <limits.h>
	#include <unistd.h>
#elif defined HE_SYSTEM_MACOS
#endif


namespace Utils {


	std::string GetCurrentDir()
	{
		return std::filesystem::current_path().string();
	}


	std::string GetExePath()
	{
		std::string path;

#if defined HE_SYSTEM_WINDOWS 

		char ctemp[MAX_PATH];
		wchar_t wtemp[MAX_PATH];
		GetModuleFileName(NULL, wtemp, MAX_PATH);
		wcstombs_s(nullptr, ctemp, wtemp, MAX_PATH);

		std::string strtemp(ctemp);
		path = strtemp.substr(0, strtemp.find_last_of("/\\"));

#elif defined HE_SYSTEM_LINUX

		// TODO... test GetExePath() for Linux
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);

#elif defined HE_SYSTEM_MACOS

		// TODO... implement GetExePath() for MacOS
		// TODO... test GetExePath() for MacOS
#pragma error "Not implemented!"

#endif
		return path;
	}


} // namespace Utils
