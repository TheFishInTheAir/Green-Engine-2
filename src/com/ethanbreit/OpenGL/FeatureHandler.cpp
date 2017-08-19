//
// Created by Ethan Breit on 2017-08-16.
//
#include <GL/glew.h>

#include <map>
#include <OpenGL/FeatureHandler.h>
#include <graphics/enums/Features.h>
#include <memory/GlobalMemory.h>

#define registerT(i) map->insert({i,true})
#define registerF(i) map->insert({i,false})
namespace ge
{
    namespace GL
    {
        using namespace Features;

        void registerFeatures(std::map<unsigned int, bool> *map)
        {
            if(GLEW_EXT_texture_filter_anisotropic) ///Anisotropy
            {
                registerT(Anisotropy);
                GLfloat anisotropy = 0;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);

                GlobalMemory::insert("ge_max_anisotropy", {new int(anisotropy) , ReadableMemType::INTEGER});

                if (anisotropy >= 4)
                    registerT(Anisotropic4);
                if (anisotropy >= 8)
                    registerT(Anisotropic8);
                if (anisotropy >= 16)
                    registerT(Anisotropic16);
            }
            else
            {
                registerF(Anisotropy);
                registerF(Anisotropic4);
                registerF(Anisotropic8);
                registerF(Anisotropic16);

            }
            if(GLEW_ARB_gl_spirv) ///Read SPIRV shaders
            {
                registerT(SPIRV_COMPILE);
            }else
                registerF(SPIRV_COMPILE);

            ///Texture Units
            {
                GLfloat maxUnits = 0;
                glGetFloatv(GL_MAX_TEXTURE_IMAGE_UNITS,&maxUnits);
                GlobalMemory::insert("ge_max_texture_units", {new int(maxUnits), ReadableMemType::INTEGER});
            }
        }

    }
}