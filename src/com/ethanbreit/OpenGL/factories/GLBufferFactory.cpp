//
// Created by Ethan Breit on 2017-07-26.
//

#include <OpenGL/factories/GLBufferFactory.h>
#include <graphics/types/IndexBuffer.h>
#include <OpenGL/types/GLIndexBuffer.h>


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