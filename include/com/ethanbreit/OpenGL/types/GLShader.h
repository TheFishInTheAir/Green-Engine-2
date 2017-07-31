//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <graphics/types/Shader.h>

namespace ge
{
    namespace GL
    {

        struct Shader : ge::Shader
        {
            ~Shader()
            {
                glDeleteShader(id);
            }

            unsigned int id;
        };
    }
}