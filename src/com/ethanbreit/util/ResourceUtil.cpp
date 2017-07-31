#include <util/ResourceUtil.h>
#include <iostream>
namespace ge
{

    namespace ResourceUtil
    {
        /*
        int writeStrResource(std::string path, std::string *fileIn)
        {
            char *pu = PROJECT_URL();
            std::ofstream file = std::ofstream(std::string(pu).append(path));
            free(pu);
            file << fileIn;
            //std::write
        }

        int getStrResource(std::string path, std::string *fileOut)
        {
            //TODO:: ERROR CONDITIONS I.E. IF FILE EXISTS
            char *pu = PROJECT_URL();
            std::ifstream file = std::ifstream(std::string(pu).append(path));
            free(pu);


            std::string str;
            while (std::getline(file, str))
            {
                *fileOut += str;
                (*fileOut).push_back('\n');
            }


            return GE_SUCCESS;

        }*/

        int getRawStrResource(std::string path, std::string *fileOut)
        {
            //TODO:: ERROR CONDITIONS I.E. IF FILE EXISTS
            std::ifstream file = std::ifstream(path);


            std::string str;
            while (std::getline(file, str))
            {
                *fileOut += str;
                (*fileOut).push_back('\n');
            }


            return 0;

        }/*

        int genResourceSystem()
        {
            char *projPath = PROJECT_URL();
            boost::filesystem::create_directories(projPath);
            free(projPath);
            PrintUtil::DebugPrint("\n");
            PrintUtil::DebugPrint(boost::filesystem::current_path().string());
            PrintUtil::DebugPrint("\n");

            return GE_SUCCESS;
        }

        bool engineSystemExists()
        {
            return boost::filesystem::exists(ENGINE_URL());
        }

        bool projectSystemExists()
        {
            return boost::filesystem::exists(PROJECT_URL());
        }

        int ResourceUtil::gotoProjectUrl()
        {

            char *path = PROJECT_URL();

            chdir(path);
            if (boost::filesystem::current_path().string() != path)
            {
                free(path);
                GE_ERR(GE_ERROR_FILESYSTEM_NOT_PRESENT, "Couldn't go to project filesystem (most likely non-existent)")
                return GE_ERROR_FILESYSTEM_NOT_PRESENT;
            }
            free(path);
            PrintUtil::DebugPrint(boost::filesystem::current_path().string());
            PrintUtil::DebugPrint("\n");
            return GE_SUCCESS;
        }

        int ResourceUtil::gotoEnginetUrl()
        {

            char *path = ENGINE_URL();

            chdir(path);
            if (boost::filesystem::current_path().string() != path)
            {
                free(path);
                GE_ERR(GE_ERROR_FILESYSTEM_NOT_PRESENT, "Couldn't go to project filesystem (most likely non-existent)")
                return GE_ERROR_FILESYSTEM_NOT_PRESENT;
            }
            free(path);
            PrintUtil::DebugPrint(boost::filesystem::current_path().string());
            PrintUtil::DebugPrint("\n");
            return GE_SUCCESS;
        }*/
    }
}