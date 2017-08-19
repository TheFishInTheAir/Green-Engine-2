//
// Created by Ethan Breit on 2017-07-29.
//

#include <graphics/enums/GraphicsApi.h>
#include <graphics/GraphicsCore.h>
#include <memory/GlobalMemory.h>

namespace ge
{

    GraphicsCore::GraphicsCore(GraphicsApi::type api)
    {
        GlobalMemory::init();

        switch (api)
        {

            case GraphicsApi::OpenGL:

                window              = new GL::Window();
                bufferFactory       = new GL::BufferFactory();
                shaderFactory       = new GL::ShaderFactory();
                meshFactory         = new GL::MeshFactory();
            case GraphicsApi::Vulkan:break;
            case GraphicsApi::Metal:break;
        }



        GlobalMemory::insert("ge_renderer_instance", GlobalMemory::MemItem(this,ReadableMemType::OTHER)); ///Add static reference to this in the global memory
    }

    bool GraphicsCore::getSupportedFeature(unsigned int feature)
    {
        return supportedFeatures.find(feature)->second;
    }
}