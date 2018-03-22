//
// Created by Ethan Breit on 2017-07-29.
//

#include <graphics/enums/GraphicsApi.h>
#include <graphics/GraphicsCore.h>
#include <memory/GlobalMemory.h>
#include "OpenGL/factories/GLTextureFactory.h"
#include "OpenGL/FeatureHandler.h"
#include <engine/global_settings.pre>
namespace ge
{

    GraphicsCore::GraphicsCore(GraphicsApi::type api)
    {

        switch (api)
        {

            case GraphicsApi::OpenGL:

                window              = new GL::Window();
                bufferFactory       = new GL::BufferFactory();
                shaderFactory       = new GL::ShaderFactory();
                meshFactory         = new GL::MeshFactory();
				textureFactory		= new GL::TextureFactory();


            case GraphicsApi::Vulkan:break;
            case GraphicsApi::Metal:break;
        }



        GlobalMemory::insert(DBL_STRINGIFY(RENDERER_INSTANCE), GlobalMemory::MemItem(this,ReadableMemType::OTHER)); ///Add static reference to this in the global memory
    }

    bool GraphicsCore::getSupportedFeature(unsigned int feature)
    {
        return supportedFeatures.find(feature)->second;
    }
}
