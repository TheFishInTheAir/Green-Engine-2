#include <ge/util/ResourceUtil.h>
#include <ge/console/Log.h>

#define DEV

#ifndef GE_RES_PATH
//#define GE_RES_PATH "../res/"
    #ifdef DEV
        #ifdef WIN32
            #define GE_RES_PATH   "C:/Users/Ethan Breit/CMakeBuilds/7f623dd2-acf9-3a38-a2a7-973bf10a64cd/build/x64-Debug/res/" //TODO: MAKE PORTABLE
        #else
            #define GE_RES_PATH    "/Users/garthbreit/Desktop/Green-Engine-2/res/" //TODO: MAKE PORTABLE
        #endif
    #endif
#endif

namespace ge
{

    namespace ResourceUtil
    {
        const std::string LOG_TAG = "ResourceUtil";

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
                Log::critErr(LOG_TAG, "Couldn't Open File: "+getResPath(path)+", "+path+"\n");
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
				Log::critErr(LOG_TAG, "Couldn't Open File: " + getResPath(path) + ", " + path + "\n");
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
        int writeRawStrResource(std::string path, std::string fileIn)
        {
            std::ofstream file = std::ofstream(getResPath(path));

            if(!file)
            {
                Log::err("Couldn't Open File: "+getResPath(path)+", "+path+"\n");
				return -1;
            }

            file << fileIn;

            file.close();
            return 0;
        }
    }
}
