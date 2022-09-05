#include "pch.h"

#include "HeliosEngine/Utils/Path.h"


// platform headers
#if defined BUILDTARGET_WINDOWS
	#include <Windows.h>
	#include <stdlib.h>
#elif defined BUILDTARGET_LINUX
	#include <string>
	#include <limits.h>
	#include <unistd.h>
#elif defined BUILDTARGET_MACOS
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

#if defined BUILDTARGET_WINDOWS 

		char ctemp[MAX_PATH];
		wchar_t wtemp[MAX_PATH];
		GetModuleFileName(NULL, wtemp, MAX_PATH);
		wcstombs_s(nullptr, ctemp, wtemp, MAX_PATH);

		std::string strtemp(ctemp);
		path = strtemp.substr(0, strtemp.find_last_of("/\\"));

#elif defined BUILDTARGET_LINUX

		// TODO... test GetExePath() for Linux
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);

#elif defined BUILDTARGET_MACOS

		// TODO... implement GetExePath() for MacOS
		// TODO... test GetExePath() for MacOS
#pragma error "Not implemented!"

#endif
		return path;
	}


} // namespace Utils
