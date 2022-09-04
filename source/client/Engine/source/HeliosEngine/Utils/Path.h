#pragma once


namespace Utils {


	// character to separate directories
	#if defined HE_SYSTEM_WINDOWS
		#define DIR_SEP "\\"
	#else
		#define DIR_SEP "/"
	#endif

	// get current working dir
	std::string GetCurrentDir();

	// get path to the executable
	std::string GetExePath();


} // namespace Utils
