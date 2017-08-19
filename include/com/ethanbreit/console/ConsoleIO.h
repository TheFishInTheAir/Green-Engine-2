//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <console/enum/MessageType.h>
#include <string>

namespace ge
{
    namespace ConsoleIO
    {


        void print(std::string);
        void Print(std::string, MessageType::type);
    }
}