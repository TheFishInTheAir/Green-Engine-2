//
// Created by Ethan Breit on 2017-07-26.
//

#include <ge/graphics/abs/OpenGL/factories/GLBufferFactory.h>


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