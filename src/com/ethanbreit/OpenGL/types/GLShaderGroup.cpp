//
// Created by Ethan Breit on 2017-07-23.
//

#include <OpenGL/types/GLShaderGroup.h>

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