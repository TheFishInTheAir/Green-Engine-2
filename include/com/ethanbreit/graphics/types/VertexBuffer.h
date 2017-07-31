//
// Created by Ethan Breit on 2017-07-25.
//

#pragma once

#include <graphics/enums/DataType.h>
#include <graphics/enums/BufferMemoryType.h>

namespace ge
{
    struct VertexBuffer
    {

        virtual ~VertexBuffer(){}

        unsigned int attributeId;
        int sizePerAttrib;
        void* offset;
        DataType::type dataType;
        bool normalized;

        virtual void bufferData(size_t, const void*, BufferMemoryType::type) = 0;

    };
}