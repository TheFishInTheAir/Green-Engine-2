//
// Created by Ethan Breit on 2017-07-29.
//

#include <OpenGL/Meshes/GLBaseTriangleMesh.h>
#include <OpenGL/factories/GLMeshFactory.h>

namespace ge
{
    namespace GL
    {
        ge::BaseTriangleMesh *MeshFactory::newTriangleMesh()
        {
            return new GL::TriangleMesh();
        }
    }
}