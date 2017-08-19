//
// Created by Ethan Breit on 2017-08-16.
//

#include <GL/glew.h>
#include <OpenGL/factories/GLTextureFactory.h>
#include <OpenGL/types/GLTexture.h>

namespace ge
{
    namespace GL
    {
        Error TextureFactory::genTexture(std::string path, Image img, ge::Texture ** outTex)
        {
            Texture *glTex = new Texture();
            glGenTextures(1,&(glTex->id));
            glBindTexture(GL_TEXTURE_2D, (glTex->id));
            unsigned int channels = 0;
            switch (img.model)
            {
                case ColourModelType::RGB:
                    channels = GL_RGB;
                    break;
                case ColourModelType::BGR:
                    channels = GL_BGR;
                    break;
                case ColourModelType::ARGB:
                    //channels = GL_ARGB; TODO: Something maybe
                    break;
                case ColourModelType::RGBA:
                    channels = GL_RGBA;
                    break;
                case ColourModelType::ABGR:
                    channels = GL_ABGR_EXT;
                    break;
                case ColourModelType::BGRA:
                    channels = GL_BGRA;
                    break;
            }
            glTexImage2D(GL_TEXTURE_2D, 0,channels, img.width, img.height, 0, channels, GL_UNSIGNED_BYTE, img.data);

            return ge::Error();
        }
    }
}
