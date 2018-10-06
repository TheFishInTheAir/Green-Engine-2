#include <ge/util/ResourceUtil.h>
#include <ge/console/Log.h>

//#include <experimental/filesystem>

//this is osx only
#include <stdlib.h>
#ifndef WIN32
#include <libproc.h>
#include <unistd.h>

#define FILE_SEP '/'

std::string _get_os_pid_bin_path()
{
    std::string path;
    
    int ret;
    pid_t pid;
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

    pid = getpid();
    ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    
    if(ret <= 0)
    {
        ge::Log::err("BADFOOD HAPPENED", "HELPHELPHELPHLEPHLPELHPELHPELPELHPLHPELHPELHPEHL");
    }
    else
    {
        ge::Log::dbg("good stuff :)", std::to_string(pid)+", '"+pathbuf+"'");
    }
    path = pathbuf;
    return path;
}
#else
#include <windows.h>
#define FILE_SEP '\\'

std::string _get_os_pid_bin_path()
{

	std::string path;

	HMODULE hModule = GetModuleHandleW(NULL);

	WCHAR tpath[260];
	GetModuleFileNameW(hModule, tpath, 260);

	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, tpath, -1, ch, 260, &DefChar, NULL);

	path = std::string(ch);

	ge::Log::dbg("good stuff :)", "FUCK: '" + path + "'");

	return path;
}
#endif

namespace ge
{

    namespace ResourceUtil
    {
        const std::string LOG_TAG = "ResourceUtil";
        std::string GE_RES_PATH;

        bool initialised = false;
        void init()
        {
            if(initialised)
                return;
            initialised=true;
            std::string path = _get_os_pid_bin_path();
            path = path.substr(0, path.find_last_of(FILE_SEP));
            path = path.substr(0, path.find_last_of(FILE_SEP));
            GE_RES_PATH = path+ FILE_SEP+ "res"+ FILE_SEP;
            Log::dbg(GE_RES_PATH);
        }

		std::string getResPath(std::string path)
		{
            init();
			return GE_RES_PATH + path;
		}

        int getRawStrResource(std::string path, std::string *fileOut)
        {
            init();
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
            init();

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
            init();
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
