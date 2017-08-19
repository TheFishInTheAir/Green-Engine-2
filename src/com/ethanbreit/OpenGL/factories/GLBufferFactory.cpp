//
// Created by Ethan Breit on 2017-07-26.
//

#include <OpenGL/CoreAbstractions/factories/GLBufferFactory.h>
#include <graphics/CoreAbstractions/types/IndexBuffer.h>
#include <OpenGL/CoreAbstractions/types/GLIndexBuffer.h>


namespace ge
{
    namespace GL
    {

        ge::VertexBuffer *ge::GL::BufferFactory::genVertexBuffer()
        {
            GL::VertexBuffer* vertexBuffer = new GL::VertexBuffer();
            return vertexBuffer;
        }
        ge::IndexBuffer *ge::GL::BufferFactory::genIndexBuffer()
        {
            GL::IndexBuffer* indexBuffer = new GL::IndexBuffer();
            return indexBuffer;
        }
    }
}