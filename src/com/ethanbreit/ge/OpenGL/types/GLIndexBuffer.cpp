//
// Created by Ethan Breit on 2017-07-26.
//

#include <ge/graphics/abs/OpenGL/types/GLIndexBuffer.h>

namespace ge
{
    namespace GL
    {
        void IndexBuffer::bufferData()
        {
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->size, data->data, EnumUtil::getTrueBufferMemoryType(data->memoryType));
        }

        void IndexBuffer::bind()
        {
            unsigned int i;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&i);

            if(i!=id)
            {
               glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            }

        }

        IndexBuffer::~IndexBuffer()
        {
            glDeleteBuffers(1,&id);
        }

        IndexBuffer::IndexBuffer()
        {
            glGenBuffers(1,&id);
        }
    }
}
