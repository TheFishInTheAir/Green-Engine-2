//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once
#include <vector>
#include <memory>
#include <ge/graphics/abs/OpenGL/types/GLShader.h>

namespace ge
{
    struct ShaderGroup
    {
		std::vector<std::shared_ptr<ge::Shader>> shaders; //TODO: MAKE GOOD
        virtual ~ShaderGroup()
        {}
    };
}
