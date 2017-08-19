//
// Created by Ethan Breit on 2017-07-26.
//

#pragma once

#include <graphics/CoreAbstractions/types/VertexBuffer.h>
#include <graphics/CoreAbstractions/types/IndexBuffer.h>

namespace ge
{
    struct BufferFactory
    {
        virtual ge::VertexBuffer *genVertexBuffer() = 0;

        virtual ge::IndexBuffer  *genIndexBuffer()  = 0;
    };
}