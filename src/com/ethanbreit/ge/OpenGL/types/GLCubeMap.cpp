//
// Created by Ethan Breit on 2017-08-16.
//

#pragma once
#include <GL/glew.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/error/Error.h>
#include <ge/graphics/abs/OpenGL/types/GLCubeMap.h>

namespace ge
{
    namespace GL
    {
        typedef unsigned char byte;


        ge::Error ge::GL::CubeMap::setFiltering(TextureFilterType::type t)
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, id);

            if((((byte)t&0xf0)>>4) == 0) /// nearest
            {
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
            else if((((byte)t&0xf0)>>4) == 1) /// linear (bilinear)
            {
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            else if((((byte)t&0xf0)>>4) == 2)/// anisotropic
            {

				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

                byte anisotropy = (byte) (pow(2,((byte)t & 0xf)));
                if(GlobalMemory::get("ge_max_anisotropy").getData<byte>() >= anisotropy)
                {
                    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT,anisotropy);
                }else
                {
                    ge_Error_GENERATE("attempt to set texture's anisotropy to ("+std::to_string(anisotropy)+") higher than graphics card limit ("+std::to_string(GlobalMemory::get("ge_max_anisotropy").getData<byte>())+")!");
                }
            }
            return Error();
        }
    }

}
