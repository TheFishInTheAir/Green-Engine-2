//
// Created by Ethan Breit on 2017-07-22.
//

#include <console/ConsoleIO.h>
#include <iostream>
//#ifndef NO_WIN_COLOUR
#include <windows.h>
//#endif

#include <util/ResourceUtil.h>

#define LOG

namespace ge
{
    namespace ConsoleIO
    {

		void RawOut(std::string m)
		{

#ifdef LOG

			static std::string logPos = ResourceUtil::getResPath("../GREEN_ENGINE_LOG.txt");
			static std::ofstream out_log(logPos);
			static bool needsSetup = true;

			out_log << m;

#endif

			std::cout << m;
		}

//TODO: do more complicated stuff with buffering to seperate consoles, I.E. stringbuf, stringstream, or vector of strings.
        void print(std::string msg) {
            RawOut(msg);
        }

        void print(std::string msg, MessageType::type t) {
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
				RawOut(msg);

				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Message: 
				RawOut(msg);
				break;
			case MessageType::Success: 
				SetConsoleTextAttribute(hConsole, 2);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Error: 
				SetConsoleTextAttribute(hConsole, 4);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Debug: 
				SetConsoleTextAttribute(hConsole, 5);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Verbose: 
				SetConsoleTextAttribute(hConsole, 7);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Turbo_Verbose: 
				SetConsoleTextAttribute(hConsole, 8);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			default:
				RawOut(msg);
			}
        	
#else
			RawOut(msg);
#endif
        }
    }
}
