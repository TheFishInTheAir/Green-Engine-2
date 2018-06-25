//
// Created by Ethan Breit on 2017-07-23.
//

#include <ge/graphics/abs/OpenGL/types/GLShaderGroup.h>

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