//
// Created by Ethan Breit on 2017-07-23.
//

#include <OpenGL/types/GLShaderGroup.h>
#include <OpenGL/types/GLShader.h>

namespace ge
{
    namespace GL
    {

        ShaderGroup::~ShaderGroup()
        {
            glDeleteProgram(programID);
        }


    }
}