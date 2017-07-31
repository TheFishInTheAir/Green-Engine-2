//
// Created by Ethan Breit on 2017-07-26.
//

#include <OpenGL/types/GLIndexBuffer.h>
#include <GL/glew.h>
#include <OpenGL/util/EnumUtil.h>

namespace ge
{
    namespace GL
    {
        void IndexBuffer::bufferData(size_t size, const void* data, BufferMemoryType::type t)
        {
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, EnumUtil::getTrueBufferMemoryType(t));

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
