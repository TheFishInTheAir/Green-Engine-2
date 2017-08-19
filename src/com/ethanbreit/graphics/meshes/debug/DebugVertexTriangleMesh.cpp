//
// Created by Ethan Breit on 2017-08-10.
//

#include <graphics/meshes/debug/DebugVertexTriangleMesh.h>
#include <memory/GlobalMemory.h>

namespace ge
{


    DebugVertexTriangleMesh::DebugVertexTriangleMesh()
    {
        _graphicsCore = GlobalMemory::get("ge_render_instance").getRawData<GraphicsCore>();
    }


    void DebugVertexTriangleMesh::rebuffer()
    {

    }

    void DebugVertexTriangleMesh::render()
    {

    }

}