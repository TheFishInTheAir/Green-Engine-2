//
// Created by Ethan Breit on 2017-07-22.
//

#include <console/ConsoleIO.h>
#include <iostream>

namespace ge
{
    namespace ConsoleIO
    {
//TODO: do more complicated stuff with buffering to seperate consoles, I.E. stringbuf, stringstream, or vector of strings.
        void Print(std::string msg) {
            std::cout << msg;
        }

        void Print(std::string msg, MessageType::type) {
            std::cout << msg;
        }
    }
}