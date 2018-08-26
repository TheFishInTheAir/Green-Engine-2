//
// Created by Ethan Breit on 2017-07-22.
//

#include <ge/console/ConsoleIO.h>
#include <iostream>
#include <mutex>
#include <gecon.h>
//#define NO_COLOUR
//#define GECON_ENABLE

#define ENABLE_TURBO_VERBOSE

//TODO: ORGANISE, TOO MANY PREPROCESSOR IFS

//TODO: make async (maybe push to update command buffer) instead of mutex

#if defined(_WIN32) && !defined(NO_COLOUR)
#include <windows.h>
#endif

#include <ge/util/ResourceUtil.h>

//#define LOG

namespace ge
{
    namespace ConsoleIO
    {

		std::mutex canPrint;

		void RawOut(std::string m)
		{
#ifdef GECON_ENABLE
#define GECON_PRINT(m, i) GECON::msg(m,i)

			static bool shouldInit = true;
			if(shouldInit)
			{
				shouldInit = false;
				GECON::startNodeChild();
				
				for(int i = 0; i < 100000*10000; i++)
					i += i-i;;;;
			}
#else
//stub out
#define GECON_PRINT(m, i)
#endif
#ifdef LOG

			static std::string logPos = ResourceUtil::getResPath("../GREEN_ENGINE_LOG.txt");
			static std::ofstream out_log(logPos);

			out_log << m;

#endif

			std::cout << m;
		}

//TODO: do more complicated stuff with buffering to seperate consoles, I.E. stringbuf, stringstream, or vector of strings.
        void print(std::string msg) 
		{
            RawOut(msg);
        }

        void print(std::string msg, MessageType::type t) {

			//canPrint.lock();
#ifndef ENABLE_TURBO_VERBOSE
			if(t==MessageType::Turbo_Verbose)
			{
				return;
			}
#endif
#ifndef NO_COLOUR

#ifdef _WIN32

			static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			switch(t)
			{
				
			
			case MessageType::Warning:          // bright red
				SetConsoleTextAttribute(hConsole, 12);
				RawOut(msg);

				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Message:          // white
				RawOut(msg);
				break;
			case MessageType::Success:          // green
				SetConsoleTextAttribute(hConsole, 2);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Error:            // dark red
				SetConsoleTextAttribute(hConsole, 4);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Debug:            // purple
				SetConsoleTextAttribute(hConsole, 5);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Verbose:          // grey
				SetConsoleTextAttribute(hConsole, 7);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case MessageType::Turbo_Verbose:    // dark grey
				SetConsoleTextAttribute(hConsole, 8);
				RawOut(msg);
				SetConsoleTextAttribute(hConsole, 15);
				break;
			default:
				RawOut(msg);
			}

#else

			switch(t)
			{


				case MessageType::Warning:          // yellow
					RawOut("\033[33m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_WARNING);
			
					break;
				case MessageType::Message:          // white
					RawOut("\033[0m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_MESSAGE);

					break;
				case MessageType::Success:          // green
					RawOut("\033[32;1m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_SUCCESS);

					break;
				case MessageType::Error:            // dark red
					RawOut("\033[31;1;7m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_ERROR);

					break;
				case MessageType::Debug:            // purple
					RawOut("\033[35m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_SUCCESS);

					break;
				case MessageType::Verbose:          // grey
					RawOut("\033[37m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_MESSAGE);

					break;
				case MessageType::Turbo_Verbose:    // dark grey
					RawOut("\033[90m"+msg+"\033[0m");
					GECON_PRINT(msg, GECON_MESSAGE);

					break;
				default:
					RawOut(msg);
			}

#endif

#else
			RawOut(msg);
#endif
			//canPrint.unlock();
        }
    }
}
