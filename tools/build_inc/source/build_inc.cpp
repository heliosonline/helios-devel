#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>


char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}


bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}



int main(int argc, char* argv[])
{
	// extract cmd line options
	char* bfile = getCmdOption(argv, argv + argc, "-bfile");
	if (!bfile) {
		std::cout << "Missing option -bfile FILENAME" << std::endl;
		return -1;
	}
	char* bdef = getCmdOption(argv, argv + argc, "-bdef");
	if (!bdef) {
		std::cout << "Missing option -bdef DEFINE" << std::endl;
		return -1;
	}


	// read file
	std::string buffer;
	std::fstream file(bfile);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			buffer.append(line);
			buffer.append("\n");
		}
		file.close();
	}
	else
	{
		std::cout << "Can't open file for read: " << bfile << std::endl;
		return -1;
	}


	// find positions
	size_t pos1 = buffer.find(bdef, 0);
	if (pos1 == std::string::npos)
		return -1;
	pos1 = buffer.find_first_of(' ', pos1);
	if (pos1 == std::string::npos)
		return -1;
	pos1 = buffer.find_first_not_of(' ', pos1);
	if (pos1 == std::string::npos)
		return -1;
	size_t pos2 = buffer.find_first_not_of("1234567890", pos1);
	if (pos2 == std::string::npos)
		return -1;


	// get current build
	int build = std::stoi(buffer.substr(pos1, pos2 - pos1));
	build++;


	// save modified file
	file.open(bfile, std::ios::out | std::ios::trunc);
	if (file.is_open())
	{
		file << buffer.substr(0, pos1);
		file << build;
		file << buffer.substr(pos2);

		file.close();
	}
	else
	{
		std::cout << "Can't open file for write: " << bfile << std::endl;
		return -1;
	}


	// information
	std::cout << bdef << " is now " << build << std::endl;


	return 0;
}
