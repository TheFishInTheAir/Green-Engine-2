//
// Created by Ethan Breit on 2017-07-23.
//

#pragma once
#include <gl/glew.h>

#include <graphics/types/ShaderGroup.h>

#include "GLVertexBuffer.h"
#include "GLUniform.h"


namespace ge
{
    namespace GL
    {
        struct ShaderGroup : ge::ShaderGroup
        {
            ~ShaderGroup();

            GLuint programID;

        };
    }
}