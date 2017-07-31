//
// Created by Ethan Breit on 2017-07-29.
//

#pragma once

#include <graphics/factories/RenderObjectFactory.h>

namespace ge
{
    namespace GL
    {
        struct RenderObjectFactory : ge::RenderObjectFactory
        {
            RenderObject *newRenderObject();
        };
    }
}