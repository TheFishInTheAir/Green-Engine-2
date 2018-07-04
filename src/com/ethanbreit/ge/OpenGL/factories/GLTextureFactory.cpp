//
// Created by Ethan Breit on 2017-08-16.
//

#include <GL/glew.h>
#include <ge/graphics/abs/OpenGL/factories/GLTextureFactory.h>
#include <ge/graphics/abs/OpenGL/types/GLTexture.h>
#include <ge/graphics/abs/OpenGL/types/GLCubeMap.h>
#include <ge/console/Log.h>

#include <array>

namespace ge
{
    namespace GL
    {
        const std::string LOG_TAG = "GLTextureFactory";
        
        unsigned int realCMT(ColourModelType::type cmt)
        {
            switch (cmt)
                {
				case ColourModelType::BW:
					return GL_RED;
                case ColourModelType::RGB:
                    return GL_RGB;
                case ColourModelType::BGR:
                    return GL_BGR;
                case ColourModelType::ARGB:
                    //channels = GL_ARGB; TODO: Something maybe @?
                    return 0;
                case ColourModelType::RGBA:
                    return GL_RGBA;
                case ColourModelType::ABGR:
                    //TODO: check if extension is supported 
                    return GL_ABGR_EXT;
                case ColourModelType::BGRA:
                    return GL_BGRA;
                default:
                    Log::critErr(LOG_TAG, "Unknown Colour Model Enum.");
                    return 0;
                }
            
        }

        Error TextureFactory::genTexture(Image img, ge::Texture ** outTex)
        {
            Texture *glTex = new Texture();
            glGenTextures(1,&(glTex->id));
            glBindTexture(GL_TEXTURE_2D, (glTex->id));
            unsigned int channels = 0;
			unsigned int coreChannel = 0;
			bool hasChannelDisconnect = false;
            channels = realCMT(img.model);
            glTexImage2D(GL_TEXTURE_2D, 0, hasChannelDisconnect ? coreChannel : channels, img.width, img.height, 0, channels, GL_UNSIGNED_BYTE, img.data);

			*outTex = glTex;
            return ge::Error();
        }

	    Error TextureFactory::genCubeMap(std::array<Image, 6> images, ge::CubeMap** outMap)
	    {

			CubeMap *glTex = new CubeMap();
			glGenTextures(1, &(glTex->id));
			glBindTexture(GL_TEXTURE_CUBE_MAP, (glTex->id));

			for (int i = 0; i < 6; i++)
			{
                Image img =  images[i];
                unsigned int channels = 0;
                unsigned int coreChannel = 0;
                bool hasChannelDisconnect = false;
                channels = realCMT(img.model);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, hasChannelDisconnect ? coreChannel : channels, img.width, img.height, 0, channels, GL_UNSIGNED_BYTE, img.data);
			}
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            *outMap = glTex;
            return ge::Error();
	    }
    }
}
