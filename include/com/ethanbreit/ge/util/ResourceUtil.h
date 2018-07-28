#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace ge
{

	namespace ResourceUtil
	{
		int getRawStrResource(std::string path, std::string *fileOut);
		int getRawStrFile(std::string path, std::string *fileOut);
        int writeRawStrResource(std::string path, std::string fileIn);
		std::string getResPath(std::string);
	}
}
