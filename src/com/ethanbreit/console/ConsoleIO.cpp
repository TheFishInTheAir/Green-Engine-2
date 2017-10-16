//
// Created by Ethan Breit on 2017-07-22.
//

#include <console/ConsoleIO.h>
#include <iostream>
//#ifndef NO_WIN_COLOUR
#include <windows.h>
//#endif

namespace ge
{
    namespace ConsoleIO
    {
//TODO: do more complicated stuff with buffering to seperate consoles, I.E. stringbuf, stringstream, or vector of strings.
        void print(std::string msg) {
            std::cout << msg;
        }

        void Print(std::string msg, MessageType::type t) {
#ifndef ENABLE_TURBO_VERBOSE
			if(t==MessageType::Turbo_Verbose)
			{
				return;
			}
#endif
#ifndef NO_WIN_COLOUR
			static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			switch(t)
			{
				
			
			case MessageType::Warning: 
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << msg;

				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Message: 
				std::cout << msg;
				break;
			case MessageType::Success: 
				SetConsoleTextAttribute(hConsole, 2);
				std::cout << msg;
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Error: 
				SetConsoleTextAttribute(hConsole, 4);
				std::cout << msg;
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Debug: 
				SetConsoleTextAttribute(hConsole, 5);
				std::cout << msg;
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Verbose: 
				SetConsoleTextAttribute(hConsole, 7);
				std::cout << msg;
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Turbo_Verbose: 
				SetConsoleTextAttribute(hConsole, 8);
				std::cout << msg;
				SetConsoleTextAttribute(hConsole, 15);
				break;
			default:
				std::cout << msg;
			}
        	
#else
            std::cout << msg;
#endif
        }
    }
}
