//
// Created by Ethan Breit on 2017-07-22.
//
#include <OpenGL/factories/GLShaderFactory.h>
#include <console/ConsoleIO.h>
#include <OpenGL/types/GLShader.h>
#include <OpenGL/util/EnumUtil.h>
#include <OpenGL/types/GLShaderGroup.h>
#include "loader/ShaderPreprocessor.h"

namespace ge
{
    namespace GL
    {

        Error ShaderFactory::genShader(std::string in_data,ge::ShaderType::type shaderType, std::shared_ptr<ge::Shader>* outShader)
        {

			std::string data = ShaderPreprocessor::process(in_data);

            GLuint id = glCreateShader(EnumUtil::getTrueShaderType(shaderType));


            ConsoleIO::print(data+"\n",MessageType::Turbo_Verbose);


            //Compile Shader

            ConsoleIO::print(std::string("Compiling Shader: ")  + " ...\n", MessageType::Verbose);

            const char * shaderCode = data.c_str();

            glShaderSource(id, 1, &shaderCode , NULL);
            glCompileShader(id);

            ConsoleIO::print(std::string("Compiled Shader: ") + "\nChecking validity ...\n", MessageType::Verbose);

            //Check

            int InfoLogLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
				std::string error_data = data;
				for (int i = 0; i < std::count(data.begin(), data.end(), '\n'); i++)
				{
					int nextLine = error_data.find_first_of('\n');
					ConsoleIO::print(std::to_string(i+1)+": "+ error_data.substr(0,nextLine+1));
					error_data = error_data.substr(nextLine + 1), error_data.length();

				}

                ge_Error_GENERATE(std::string("Error in Shader \"oh wait I did this badly so I don't know the shader name\"\n")+&ShaderErrorMessage[0]);


            }

            ConsoleIO::print("\nValid\n\n");

            GL::Shader *shader = new GL::Shader();
            shader->id = id;
            *outShader = std::shared_ptr<ge::Shader>(shader);


            return Error();
        }

        Error ShaderFactory::genShaderGroup(std::vector<std::shared_ptr<ge::Shader>> shadersIn, ge::ShaderGroup ** outShaderGroup)
        {
            GL::ShaderGroup* shaderGroup = new GL::ShaderGroup();
            shaderGroup->programID = glCreateProgram();

            for(unsigned long i = 0; i < shadersIn.size(); i++)
            {
                glAttachShader(shaderGroup->programID, ((GL::Shader*)shadersIn.at(i).get())->id);
            }

            glLinkProgram(shaderGroup->programID);

            int InfoLogLength;
            glGetProgramiv(shaderGroup->programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(shaderGroup->programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                ge_Error_GENERATE(&ProgramErrorMessage[0]);
            }

            for(unsigned long i = 0; i < shadersIn.size(); i++)
            {
                glDetachShader(shaderGroup->programID, ((GL::Shader*)shadersIn.at(i).get())->id);
            }

			shaderGroup->shaders = shadersIn;

            *outShaderGroup = shaderGroup;

            return Error();
        }

	    void ShaderFactory::destroyShader(ge::Shader* shader)
	    {
			glDeleteShader(((GL::Shader*)shader)->id);
			free((GL::Shader*)shader);
	    }
    }
}
