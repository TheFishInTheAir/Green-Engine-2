//
// Created by Ethan Breit on 2017-07-29.
//

#pragma once

#include <graphics/meshes/BaseTriangleMesh.h>

namespace ge
{
    struct MeshFactory
    {
        virtual BaseTriangleMesh* newTriangleMesh() = 0;
    };
}