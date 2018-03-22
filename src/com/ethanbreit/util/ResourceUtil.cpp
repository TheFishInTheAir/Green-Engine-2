#include <util/ResourceUtil.h>
#include <console/ConsoleIO.h>

#ifndef GE_RES_PATH
//#define GE_RES_PATH "../res/"
#define GE_RES_PATH   "C:/Users/Ethan Breit/CMakeBuilds/7f623dd2-acf9-3a38-a2a7-973bf10a64cd/build/x64-Debug/res/"
#endif
namespace ge
{

    namespace ResourceUtil
    {

		std::string getResPath(std::string path)
		{
			return GE_RES_PATH + path;
		}

        int getRawStrResource(std::string path, std::string *fileOut)
        {
            //TODO: ERROR STANDARDIZATION PLEASE
            std::ifstream file = std::ifstream(getResPath(path));

            if(!file)
            {
                ConsoleIO::print("Couldn't Open File: "+getResPath(path)+", "+path+"\n",MessageType::Error);
				return -1;
            }

            std::string str;
            while (std::getline(file, str))
            {
                *fileOut += str;
                (*fileOut).push_back('\n');
            }


            return 0;

        }
		int getRawStrFile(std::string path, std::string *fileOut)
		{

			std::ifstream file = std::ifstream(path);

			if (!file)
			{
				ConsoleIO::print("Couldn't Open File: " + getResPath(path) + ", " + path + "\n", MessageType::Error);
				return -1;

			}

			std::string str;
			while (std::getline(file, str))
			{
				*fileOut += str;
				(*fileOut).push_back('\n');
			}


			return 0;

		}
    }
}