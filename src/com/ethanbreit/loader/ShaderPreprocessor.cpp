#include <string>
#include <regex>
#include <loader/ShaderPreprocessor.h>
#include "graphics/meshes/debug/DebugVertexTriangleMesh.h"
#include "util/ResourceUtil.h"

namespace ge
{
	namespace ShaderPreprocessor
	{
		std::regex include("#pragma include(<| <).*>");
		std::regex include_path("<.*>");
		//TODO: better name for both class and function @REFACTOR
		std::string process(std::string in)
		{
			///INCLUDE
			{
				std::smatch m;
				int includes = 0;
				while (std::regex_search(in, m, include))
				{
					if (includes > ge_MAX_PREPROCESSOR_INCLUDES)
					{
						ge_Error_ADDTRACE(ge::Error("Shader Preprocessor Include Limit Reached."));
					}
					//Get Answer
					std::string inc;
					inc = m.str();
					
					in.erase(m.position(), m.length());

					//Get String
					std::smatch path;
					std::regex_search(inc, path, include_path);
					
					//Get Path
					std::string real_path = path.str().substr(1,path.str().length()-2);
					std::string included_file;
					
					//TODO: fix the return value to Error @?
					ResourceUtil::getRawStrResource(real_path, &included_file);

					in.insert(m.position(), included_file);

					includes++;
				}

				//TODO: MORE @UNFINISHED 

			}
			return in;
		}
	}
}
