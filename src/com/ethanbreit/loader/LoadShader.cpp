//
// Created by Ethan Breit on 2017-12-23.
//

#include <error/Error.h>
#include <console/ConsoleIO.h>
#include <util/ResourceUtil.h>

#ifdef _WIN32

#define DIR_SEP '\\'

#else

#define DIR_SEP '/'

#endif

namespace ge
{

    namespace ShaderLoader
    {


        Error LoadShader(std::string path)
        {

            std::string realPath = ResourceUtil::getResPath(path);

            std::string dir = realPath.substr(0,realPath.find_last_of(DIR_SEP)+1);
            ConsoleIO::print(dir+"\n");



            return Error();
        }
    }
}