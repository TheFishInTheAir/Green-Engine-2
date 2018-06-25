//
// Created by Ethan Breit on 2017-07-29.
//

#include <ge/graphics/enums/GraphicsApi.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/graphics/abs/OpenGL/factories/GLTextureFactory.h>
#include <ge/graphics/abs/OpenGL/FeatureHandler.h>
#include <engine/global_settings.pre>
#include <ge/console/Log.h>

namespace ge
{
	GraphicsCore* GraphicsCore::ctx;

	const std::string LOG_TAG = "GraphicsCore";

    GraphicsCore::GraphicsCore(GraphicsApi::type api)
    {

		GraphicsCore::staticInit();
		
		if (ctx == nullptr)
			ctx = this;
		else
			Log::critErr(LOG_TAG, "Attempt to create new Graphics Core (One has already been created).");

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

		ctx = this;
    }

	void GraphicsCore::staticInit()
    {
		static bool first = true;
		if(first)
		{
			first = false;
			GraphicsCore::ctx = nullptr;
		}
    }

    bool GraphicsCore::getSupportedFeature(unsigned int feature)
    {
        return supportedFeatures.find(feature)->second;
    }
}
