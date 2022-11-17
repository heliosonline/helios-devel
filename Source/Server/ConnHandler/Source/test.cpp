#include "pch.h"

#include "config/version.h"


int main()
{
	std::cout << "Helios::ConnectionHandler [Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "." << VERSION_BUILD << "]" << std::endl;
	std::cout << "Copyright (C) 2022, Lennart Molnar <pernicius@web.de>" << std::endl;
	std::cout << "See LICENSE.md for further details." << std::endl;
	std::cout << std::endl;
}
