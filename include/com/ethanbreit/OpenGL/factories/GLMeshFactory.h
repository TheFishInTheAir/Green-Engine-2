//
// Created by Ethan Breit on 2017-07-29.
//

#pragma once

#include <graphics/factories/MeshFactory.h>

namespace ge
{
    namespace GL
    {
        struct MeshFactory : ge::MeshFactory
        {
            ge::TriangleMesh *newTriangleMesh();
        };
    }
}