//
// Created by Ethan Breit on 2017-07-22.
//
#include <OpenGL/CoreAbstractions/factories/GLShaderFactory.h>
#include <console/ConsoleIO.h>
#include <OpenGL/CoreAbstractions/types/GLShader.h>
#include <OpenGL/util/EnumUtil.h>
#include <OpenGL/CoreAbstractions/types/GLShaderGroup.h>

namespace ge
{
    namespace GL
    {

        Error ShaderFactory::genShader(std::string data,ge::ShaderType::type shaderType, ge::Shader** outShader)
        {

            GLuint id = glCreateShader(EnumUtil::getTrueShaderType(shaderType));


            ConsoleIO::Print(data+"\n",MessageType::Turbo_Verbose);


            //Compile Shader

            ConsoleIO::Print(std::string("Compiling Shader: ")  + " ...\n", MessageType::Verbose);

            const char * shaderCode = data.c_str();

            glShaderSource(id, 1, &shaderCode , NULL);
            glCompileShader(id);

            ConsoleIO::Print(std::string("Compiled Shader: ") + "\nChecking validity ...\n", MessageType::Verbose);

            //Check

            int InfoLogLength;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
            if ( InfoLogLength > 0 ){
                std::vector<char> ShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
                ge_Error_GENERATE(&ShaderErrorMessage[0]);
            }

            ConsoleIO::print("\nValid\n\n");

            GL::Shader *shader = new GL::Shader();
            shader->id = id;
            *outShader = (ge::Shader*)shader;


            return Error();
        }

        Error ShaderFactory::genShaderGroup(std::vector<ge::Shader *> shadersIn, ge::ShaderGroup ** outShaderGroup)
        {
            GL::ShaderGroup* shaderGroup = new GL::ShaderGroup();
            shaderGroup->programID = glCreateProgram();

            for(unsigned long i = 0; i < shadersIn.size(); i++)
            {
                glAttachShader(shaderGroup->programID, ((GL::Shader*)shadersIn.at(i))->id);
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
                glDetachShader(shaderGroup->programID, ((GL::Shader*)shadersIn.at(i))->id);
            }

            *outShaderGroup = shaderGroup;

            return Error();
        }

    }
}