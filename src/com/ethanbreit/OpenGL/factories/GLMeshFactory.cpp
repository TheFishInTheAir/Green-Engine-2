//
// Created by Ethan Breit on 2017-07-29.
//

#include <OpenGL/Meshes/GLTriangleMesh.h>
#include <OpenGL/CoreAbstractions/factories/GLMeshFactory.h>

namespace ge
{
    namespace GL
    {
        ge::TriangleMesh *MeshFactory::newTriangleMesh()
        {
            return new GL::TriangleMesh();
        }
    }
}