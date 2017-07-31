#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace ge
{

	namespace ResourceUtil
	{
		int getStrResource(std::string, std::string *);

		int writeStrResource(std::string, std::string *);

		int getRawStrResource(std::string path, std::string *fileOut);

		int genResourceSystem(void);

		int gotoProjectUrl();

		bool engineSystemExists();

		bool projectSystemExists();

		int gotoEnginetUrl();

	}
}
