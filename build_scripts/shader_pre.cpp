#include <regex>
#include <iostream>
#include <fstream>
//STANDALONE SHADER PRE PROCESSOR



int main(int argc, char *argv[])
{

	std::regex include("#pragma include(<| <).*>");
	std::regex include_path("<.*>");

	if (argc == 2 || argc == 3)
	{
		std::string in_path = argv[1];

		//std::cout << in_path << std::endl;

		std::string fuck_my_life = "C:/Users/Ethan Breit/Documents/Green Engine 2/res/";

		std::string in;

		{

			std::ifstream includeFile(in_path);
			std::string line;
			while (std::getline(includeFile, line))
			{
				in += line + '\n';
			}
			includeFile.close();

		}



		///INCLUDE
		std::smatch m;
		int includes = 0;
		while (std::regex_search(in, m, include))
		{
			if (includes > 20) //TODO: import proper thing
			{
				std::cout << "EXCEDED INCLUDE LIMIT" << std::endl;
				std::exit(1);
			}
			//Get Answer
			std::string inc;
			inc = m.str();

			in.erase(m.position(), m.length());

			//Get String
			std::smatch path;
			std::regex_search(inc, path, include_path);

			//Get Path
			std::string real_path = path.str().substr(1, path.str().length() - 2);
			std::string included_file = "";
			std::string fucking_hell = fuck_my_life+real_path;
			//Read file
			std::cout << fucking_hell << std::endl;
			{
				
				std::ifstream includeFile(fucking_hell);
				std::string line;
				while (std::getline(includeFile, line))
				{
					included_file += line + '\n';
				}
				includeFile.close();

			}


			in.insert(m.position(), included_file);

			includes++;
		}

		std::cout << in << std::endl;

		if(argc == 3)
		{
			std::ofstream outfile(argv[2]);

			outfile << in;

			outfile.close();

		}

	}
	else
	{
		std::cout << "Wrong number of arguments provided. (only expecting one)" << std::endl;
	}

}