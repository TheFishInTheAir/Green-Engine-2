//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <graphics/factories/ShaderFactory.h>
#include <graphics/types/Window.h>
#include <graphics/factories/BufferFactory.h>
#include <graphics/factories/RenderObjectFactory.h>
#include <graphics/enums/GraphicsApi.h>

namespace ge
{
    struct GraphicsCore
    {

        GraphicsCore(GraphicsApi::type);

        ShaderFactory       *shaderFactory;
        BufferFactory       *bufferFactory;
        RenderObjectFactory *renderObjectFactory;

        Window              *window;


    };
}