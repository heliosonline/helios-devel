#pragma once


namespace Utils {


	// character to separate directories
	#if defined BUILDTARGET_WINDOWS
		#define DIR_SEP "\\"
	#else
		#define DIR_SEP "/"
	#endif

	// get current working dir
	std::string GetCurrentDir();
	// set current working dir
	void SetCurrentDir(std::string& path);

	// get path to the executable
	std::string GetExePath();


} // namespace Utils
