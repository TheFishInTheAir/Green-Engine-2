//
// Created by Ethan Breit on 2017-07-29.
//

#pragma once

#include <graphics/types/RenderObject.h>

namespace ge
{
    struct RenderObjectFactory
    {
        virtual RenderObject* newRenderObject() = 0;
    };
}