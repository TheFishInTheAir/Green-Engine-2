//
// Created by Ethan Breit on 2017-07-27.
//

#pragma once

#include <graphics/enums/BufferMemoryType.h>
#include <cstddef>

namespace ge
{
    struct IndexBuffer
    {
        virtual ~IndexBuffer(){}
        unsigned int length;

        virtual void bufferData(size_t, const void*, BufferMemoryType::type) = 0;
    };
}