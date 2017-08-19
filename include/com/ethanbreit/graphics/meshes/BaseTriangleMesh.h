//
// Created by Ethan Breit on 2017-07-26.
//

#pragma once

#include <graphics/base_abstractions/types/VertexBuffer.h>
#include <graphics/base_abstractions/types/Uniform.h>
#include "graphics/base_abstractions/types/ShaderGroup.h"
#include "graphics/base_abstractions/types/IndexBuffer.h"

namespace ge
{
    struct TriangleMesh
    {
        virtual void render() = 0;
        virtual ~TriangleMesh(){};

        virtual void rebuffer() = 0;

        virtual void setShaderGroup(ge::ShaderGroup*) = 0;
        virtual ge::ShaderGroup* getShaderGroup() = 0;

        virtual void setIndexBuffer(ge::IndexBuffer*) = 0;
        virtual ge::IndexBuffer* getIndexBuffer() = 0;


        virtual void registerUniform(std::string) = 0;
        virtual void registerVertexBuffer(std::string, ge::VertexBuffer*) = 0;

        virtual ge::Uniform* getUniform(std::string) = 0;
        virtual ge::VertexBuffer* getVertexBuffer(std::string) = 0;


    };
}