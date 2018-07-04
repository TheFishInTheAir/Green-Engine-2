#include <ge/console/Log.h>

#include <ge/console/ConsoleIO.h>
#include <ge/INFO.h>
#include <cstdlib>

namespace ge
{
	namespace Log
	{
		const std::string DEFAULT_TAG = "GE";

		void msg(std::string t)
		{
			msg(DEFAULT_TAG, t);
		}

		void msg(std::string tag, std::string t)
		{
			ConsoleIO::print("M  [" + tag + "]: " + t+'\n', MessageType::Message);
		}



		void scc(std::string msg)
		{
			scc(DEFAULT_TAG, msg);
		}

        void scc(std::string tag, std::string msg)
		{
			ConsoleIO::print("S  [" + tag + "]: " + msg+'\n', MessageType::Success);
		}



		void dbg(std::string msg)
		{
			dbg(DEFAULT_TAG, msg);
		}

        void dbg(std::string tag, std::string msg)
		{
			ConsoleIO::print("D  [" + tag + "]: " + msg+'\n', MessageType::Debug);
		}



		void wrn(std::string msg)
		{
			wrn(DEFAULT_TAG, msg);
        }

        void wrn(std::string tag, std::string msg)
		{
			ConsoleIO::print("W  [" + tag + "]: " + msg+'\n', MessageType::Warning);
		}



		void err(std::string msg)
		{
			err(DEFAULT_TAG, msg);
		}

        void err(std::string tag, std::string msg)
		{
            ConsoleIO::print("E  [" + tag + "]: " + msg+'\n', MessageType::Error);
		}



		void critErr(std::string msg)
        {
            critErr(DEFAULT_TAG, msg);
        }

		void critErr(std::string tag, std::string msg) //TODO: get stack trace and stuff
        {
            ConsoleIO::print("CE [" + tag + "]: " + msg+'\n', MessageType::Error);
            std::exit(ERROR_EXIT_CODE);
        }



		void vrb(std::string msg)
        {
            vrb(DEFAULT_TAG, msg);
        }

		void vrb(std::string tag, std::string msg)
        {
            ConsoleIO::print("V  [" + tag + "]: " + msg+'\n', MessageType::Verbose);
        }



		void tVrb(std::string msg)
        {
            tVrb(DEFAULT_TAG, msg);
        }

		void tVrb(std::string tag, std::string msg)
        {
            ConsoleIO::print("TV [" + tag + "]: " + msg+'\n', MessageType::Turbo_Verbose);
        }

	}
}
