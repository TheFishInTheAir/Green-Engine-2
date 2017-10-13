//
// Created by Ethan Breit on 2017-07-29.
//

#include <OpenGL/Meshes/GLTriangleMesh.h>
#include <OpenGL/factories/GLMeshFactory.h>

namespace ge
{
    namespace GL
    {
        ge::TriangleMesh *MeshFactory::newTriangleMesh()
        {
            return new GL::TriangleMesh();
        }
		ge::TriangleMesh* MeshFactory::newTriangleMesh(Empty::MeshData m)
		{
			ge::TriangleMesh* tm = new GL::TriangleMesh();

			if (m.hasShaderGroup)
			{
				//TODO:: IMPLEMENT SOMETHING BECAUSE THIS DOESN't ACTUALLY WORK..
				tm->setShaderGroup(m.shaderGroup);
			}

			if (m.hasIndexBuffer)
			{
				ge::IndexBuffer* index = new GL::IndexBuffer();
				index->data = m.indexBuffer;
				index->bufferData();
				tm->setIndexBuffer(index);
			}

			if(m.vertexBuffers.size()!=0)
			{
				for(auto i : m.vertexBuffers)
				{
					GL::VertexBuffer* vertex = new GL::VertexBuffer();
					vertex->data = i.first;
					vertex->bufferData();
					tm->registerVertexBuffer(i.second, vertex);
				}
			}


			return tm;

		}
    }
}