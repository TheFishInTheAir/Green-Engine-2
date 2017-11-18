//
// Created by Ethan Breit on 2017-07-22.
//

#include <string>
#include <error/Error.h>
#include <chrono>
#include <util/PreprocessorUtil.h>
#include <console/ConsoleIO.h>
#include <util/StrUtil.h>

namespace ge
{

    Error::Error(std::string err) {
        _caught = false;
        time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string time = ctime(&t);
        StringUtil::removeChar(&time, '\n');
        _msg = "Error("+time+") \\/\\/\\/\n"+err+"\n";
        _isError = true;
    }

    Error::Error()
    {
        _isError = false;
        _caught = true;
    }

    Error::~Error() {
        if(!_caught)
        {
            std::string str = "";
            for(std::string s : stack)
                str += s;
            ConsoleIO::print("Uncaught "+_msg+"Stack Trace:\n"+str,MessageType::Error);
        }
    }

    bool Error::isError() {
        return _isError;
    }

}