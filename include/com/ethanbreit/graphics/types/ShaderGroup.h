//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <vector>
#include <map>
#include <error/Error.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "Uniform.h"

namespace ge
{
    struct ShaderGroup
    {
        virtual ~ShaderGroup()
        {}
    };
}