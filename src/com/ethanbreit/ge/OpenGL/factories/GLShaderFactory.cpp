//
// Created by Ethan Breit on 2017-07-22.
//
#include <ge/graphics/abs/OpenGL/factories/GLShaderFactory.h>
#include <ge/console/ConsoleIO.h>
#include <ge/graphics/abs/OpenGL/types/GLShader.h>
#include <ge/graphics/abs/OpenGL/util/EnumUtil.h>
#include <ge/graphics/abs/OpenGL/types/GLShaderGroup.h>
#include <ge/graphics/types/Uniform.h>
#include <ge/loader/ShaderPreprocessor.h>
#include <ge/console/Log.h>

namespace ge
{
    namespace GL
    {

        const std::string LOG_TAG = "GLShaderFactory";
        
        Error ShaderFactory::genShader(std::string in_data,ge::ShaderType::type shaderType, std::shared_ptr<ge::Shader>* outShader)
        {

			std::string data = ShaderPreprocessor::process(in_data);

            GLuint id = glCreateShader(EnumUtil::getTrueShaderType(shaderType));


            //Compile Shader

            Log::msg(LOG_TAG, std::string("Compiling Shader")  + " ..."); //TODO: maybe make vrb

            const char * shaderCode = data.c_str();

            glShaderSource(id, 1, &shaderCode , NULL);
            glCompileShader(id);

            Log::msg(LOG_TAG, std::string("Compiled Shader"));
            Log::msg(LOG_TAG, "Checking validity ...");

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
					Log::err(LOG_TAG, std::to_string(i+1)+": "+ error_data.substr(0,nextLine+1));
					error_data = error_data.substr(nextLine + 1), error_data.length();

				}

                ge_Error_GENERATE(std::string("Error in Shader \"oh wait I did this badly so I don't know the shader name\"\n")+&ShaderErrorMessage[0]);


            }

            Log::scc(LOG_TAG, "Valid");

            GL::Shader *shader = new GL::Shader();
            shader->id = id;
            *outShader = std::shared_ptr<ge::Shader>(shader);


            return Error();
        }

        Error ShaderFactory::genShaderGroup(std::vector<std::shared_ptr<ge::Shader>> shadersIn, ge::ShaderGroup ** outShaderGroup)//TODO: don't use Error also maybe don't use this function.
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
            //shaderGroup->uniforms = std::unordered_map<std::string, ge::Uniform>();
            
            
            { //Uniforms
                /*GLint count;
                char *name = (char*) malloc(64);
                int nameLen = 0;
                glGetProgramiv(shaderGroup->programID, GL_ACTIVE_UNIFORMS, &count);
                for(int i = 0; i < count; i++)
                {
                    glGetActiveUniform(shaderGroup->programID, i, 64, &nameLen, nullptr, nullptr, name);
                    Log::scc(LOG_TAG, std::string()+"test "+name);
                    shaderGroup->uniforms.insert({std::string(name), ge::Uniform()});
                }*/
            }
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
