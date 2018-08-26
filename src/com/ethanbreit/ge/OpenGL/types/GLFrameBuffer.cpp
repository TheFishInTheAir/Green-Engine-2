#include <GL/glew.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/error/Error.h>
#include <ge/graphics/abs/OpenGL/types/GLFrameBuffer.h>
#include <ge/console/Log.h>
namespace ge
{
    namespace GL
    {
        const std::string LOG_TAG = "GLFrameBuffer";

        void FrameBuffer::init(uint32_t width, uint32_t height)
        {
            this->width = width;
            this->height = height;
            glGenFramebuffers(1, &id);

            bind();
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            bindDefault();
        }

        void FrameBuffer::bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, id);
            glViewport(0,0, width, height);
        }

        void FrameBuffer::bindDefault()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        uint32_t FrameBuffer::getHeight()
        {
            return height;
        }

        uint32_t FrameBuffer::getWidth()
        {
            return width;
        }

        void FrameBuffer::deepDestroy()
        {
            glDeleteFramebuffers(1, &id);
            //TODO: put Texture destructor in the Texture class ffs...
            for(auto tex : textureAttachments)
                glDeleteTextures(1, &(tex.second->id));
            textureAttachments.clear();
            
            for(auto b : bufferAttachments)
                glDeleteBuffers(1, &(b.second));
            textureAttachments.clear();
        }

        ge::Texture* FrameBuffer::getTextureAttachment(FrameBufferAttachment::texture t)
        {
            if(!textureAttachments.count(t))
                Log::critErr(LOG_TAG, "Framebuffer doesn't have texture attachment '"+std::to_string(t)+"' (gl id:'"+std::to_string(id)+"')");
            return (ge::Texture*) textureAttachments[t];
        }

        void FrameBuffer::shallowDestroy()
        {
            glDeleteFramebuffers(1, &id);
            //TODO: put Texture destructor in the Texture class ffs...
            
            for(auto b : bufferAttachments)
                glDeleteTextures(1, &(b.second));
            textureAttachments.clear();
        }
        uint32_t getComponentType(FrameBufferAttachment::buffer t)
        {
            switch(t)//TODO: add others
            {
                case FrameBufferAttachment::Depth24_Stencil8:
                    return GL_DEPTH24_STENCIL8;
            }
        }
        void FrameBuffer::generateBufferAttachment(FrameBufferAttachment::buffer t) //TODO: make more dynamic and stuff...
        {
            if(bufferAttachments.count(t))
                Log::critErr(LOG_TAG, "Buffer attachment '"+std::to_string(t)+"' already in framebuffer (gl id:'"+std::to_string(id)+"')"); //yall done fucked up
            bind();

            unsigned int bid;
            glGenRenderbuffers(1, &bid);
            glBindBuffer(GL_RENDERBUFFER, bid);
            glRenderbufferStorage(GL_RENDERBUFFER, getComponentType(t), width, height);

            bufferAttachments.insert({t, bid});
            
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                GL_RENDERBUFFER, bid);

            _hasDepth = true;

            bindDefault();
        }

        uint32_t getComponentType(FrameBufferAttachment::texture t)
        {
            switch(t)
            {
                case FrameBufferAttachment::Colour:
                    return GL_RGB; //TODO: add more variety in colour data
                case FrameBufferAttachment::Depth:
                    return GL_DEPTH_COMPONENT;
                case FrameBufferAttachment::Depth16:
                    return GL_DEPTH_COMPONENT16;
                case FrameBufferAttachment::Depth24:
                    return GL_DEPTH_COMPONENT24;
                case FrameBufferAttachment::Depth32:
                    return GL_DEPTH_COMPONENT32;
            }
        }

        bool FrameBuffer::hasDepth()
        {
            return _hasDepth;
        }
        bool FrameBuffer::hasColour()
        {
            return _hasColour;
        }

        void FrameBuffer::clear()
        {
            //NOTE: there isn't a clear colour for this...
            glClearColor(1.0f,0.0f,1.0f, 1.0f);
            glClear( (_hasColour ? GL_COLOR_BUFFER_BIT : 0) | (_hasDepth ? GL_DEPTH_BUFFER_BIT : 0) );
        }

        void FrameBuffer::generateTextureAttachment(FrameBufferAttachment::texture t)
        {
            if(textureAttachments.count(t))
                Log::critErr(LOG_TAG, "Texture attachment '"+std::to_string(t)+"' already in framebuffer (gl id:'"+std::to_string(id)+"')"); //yall done fucked up

            bind();

            ge::GL::Texture* tex = new ge::GL::Texture();

            glGenTextures(1,&(tex->id));
            glBindTexture(GL_TEXTURE_2D, (tex->id));        

            uint32_t component = getComponentType(t);

            glTexImage2D(GL_TEXTURE_2D, 0, component,
                        width, height, 0, component, GL_FLOAT, NULL);
            
            tex->setFiltering(TextureFilterType::Anisotropic_16x);

            textureAttachments.insert({t, tex});

            if(t==FrameBufferAttachment::Colour)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, 0);

                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                glReadBuffer(GL_COLOR_ATTACHMENT0);
                _hasColour = true;
            }
            else if( ( ((char)t>>4) & 0x0f ) == 1) //if t == 0x1?
            {
                _hasDepth = true;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->id, 0);
            }

            bindDefault();
        }
    }
}
