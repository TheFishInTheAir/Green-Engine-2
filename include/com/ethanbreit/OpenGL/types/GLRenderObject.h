//
// Created by Ethan Breit on 2017-07-26.
//

#pragma once

#include <graphics/types/RenderObject.h>
#include "GLVertexBuffer.h"
#include "GLUniform.h"
#include "GLShaderGroup.h"
#include "GLIndexBuffer.h"

namespace ge
{
    namespace GL
    {
        struct RenderObject : ge::RenderObject
        {
            GL::ShaderGroup* shaderGroup;
            GL::IndexBuffer* indexBuffer;

            RenderObject();
            ~RenderObject();
            void render();

            void rebuffer();

            void setIndexBuffer(ge::IndexBuffer*);
            ge::IndexBuffer* getIndexBuffer();

            void setShaderGroup(ge::ShaderGroup*);
            ge::ShaderGroup* getShaderGroup();

            void registerUniform(std::string);

            void registerVertexBuffer(std::string, ge::VertexBuffer *);

            ge::Uniform* getUniform(std::string);

            ge::VertexBuffer *getVertexBuffer(std::string);
        private:

            unsigned int _vao;

            std::map<std::string,ge::GL::VertexBuffer*> _vertexBuffers;
            std::map<std::string,ge::GL::Uniform*> _uniforms;
        };
    }
}