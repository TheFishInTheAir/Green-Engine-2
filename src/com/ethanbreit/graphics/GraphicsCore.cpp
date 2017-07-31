//
// Created by Ethan Breit on 2017-07-29.
//

#include <graphics/enums/GraphicsApi.h>
#include <graphics/GraphicsCore.h>
#include <OpenGL/types/GLWindow.h>
#include <OpenGL/factories/GLBufferFactory.h>
#include <OpenGL/factories/GLShaderFactory.h>
#include <OpenGL/factories/GLRenderObjectFactory.h>

namespace ge
{

    GraphicsCore::GraphicsCore(GraphicsApi::type api)
    {
        switch (api)
        {

            case GraphicsApi::API_OpenGL:

                window              = new GL::Window();
                bufferFactory       = new GL::BufferFactory();
                shaderFactory       = new GL::ShaderFactory();
                renderObjectFactory = new GL::RenderObjectFactory();

                return;
            case GraphicsApi::API_Vulkan:break;
            case GraphicsApi::API_Metal:break;
        }
    }
}