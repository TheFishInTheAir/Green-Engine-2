//
// Created by Ethan Breit on 2017-07-26.
//

#include <OpenGL/types/GLVertexBuffer.h>

namespace ge
{
    namespace GL
    {
        void VertexBuffer::bufferData()
        {
            bind();
            glBufferData(GL_ARRAY_BUFFER, data->size, data->data, EnumUtil::getTrueBufferMemoryType(data->memoryType));

        }

        void VertexBuffer::bind()
        {
            unsigned int i;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&i);

            if(i!=id)
            {
               glBindBuffer(GL_ARRAY_BUFFER, id);
            }

        }

        VertexBuffer::~VertexBuffer()
        {
            glDeleteBuffers(1,&id);
        }

        VertexBuffer::VertexBuffer()
        {
            glGenBuffers(1,&id);
        }
    }
}
