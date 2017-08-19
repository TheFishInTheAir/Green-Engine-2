//
// Created by Ethan Breit on 2017-07-26.
//

#pragma once

#include <GL/glew.h>

#include <graphics/factories/BufferFactory.h>
#include <graphics/base_abstractions/types/VertexBuffer.h>

#include <OpenGL/base_abstractions/types/GLVertexBuffer.h>
#include <graphics/base_abstractions/types/IndexBuffer.h>


namespace ge
{
    namespace GL
    {
        struct BufferFactory : ge::BufferFactory
        {
            ge::VertexBuffer *genVertexBuffer();

            ge::IndexBuffer  *genIndexBuffer();
        };
    }
}