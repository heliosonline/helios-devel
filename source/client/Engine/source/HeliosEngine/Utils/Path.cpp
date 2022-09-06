#include "pch.h"

#include "HeliosEngine/Utils/Path.h"


// platform headers
#if defined TARGET_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <stdlib.h>
#elif defined TARGET_PLATFORM_LINUX
	#include <string>
	#include <limits.h>
	#include <unistd.h>
#elif defined TARGET_PLATFORM_MACOS
#endif


namespace Utils {


	std::string GetCurrentDir()
	{
		return std::filesystem::current_path().string();
	}


	void SetCurrentDir(std::string& path)
	{
		std::filesystem::current_path(path);
	}


	std::string GetExePath()
	{
		std::string path;

#if defined TARGET_PLATFORM_WINDOWS 

		char ctemp[MAX_PATH];
		wchar_t wtemp[MAX_PATH];
		GetModuleFileName(NULL, wtemp, MAX_PATH);
		wcstombs_s(nullptr, ctemp, wtemp, MAX_PATH);

		std::string strtemp(ctemp);
		path = strtemp.substr(0, strtemp.find_last_of("/\\"));

#elif defined TARGET_PLATFORM_LINUX

		// TODO... test GetExePath() for Linux
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);

#elif defined TARGET_PLATFORM_MACOS

		// TODO... implement GetExePath() for MacOS
		// TODO... test GetExePath() for MacOS
		#pragma error "Not implemented!"

#endif
		return path;
	}


} // namespace Utils
