//
// Created by Ethan Breit on 2017-07-26.
//

#pragma once

#include "ShaderGroup.h"
#include "IndexBuffer.h"

namespace ge
{
    struct RenderObject
    {
        virtual void render() = 0;
        virtual ~RenderObject(){};

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