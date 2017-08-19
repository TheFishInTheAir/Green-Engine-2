//
// Created by Ethan Breit on 2017-08-10.
//

#pragma once

#include <error/Error.h>
#include <graphics/meshes/BaseTriangleMesh.h>

namespace ge
{

    namespace MeshLoader
    {
        Error loadTriangleMesh(std::string path,BaseTriangleMesh**);
    }

}