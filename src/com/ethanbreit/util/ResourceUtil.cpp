#include <util/ResourceUtil.h>
#include <console/ConsoleIO.h>

#ifndef GE_RES_PATH
#define GE_RES_PATH "../res/"
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
            //TODO: ERROR CONDITIONS I.E. IF FILE EXISTS
            std::ifstream file = std::ifstream(getResPath(path));

            if(!file)
            {
                ConsoleIO::print("Couldn't Open File: "+getResPath(path)+", "+path+"\n",MessageType::Error);
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