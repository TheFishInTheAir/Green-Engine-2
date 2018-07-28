//
// Created by Ethan Breit on 2017-07-29.
//

#include <ge/graphics/abs/OpenGL/Meshes/GLTriangleMesh.h>
#include <ge/graphics/abs/OpenGL/factories/GLMeshFactory.h>
#include <ge/console/Log.h>
#include <cstring>

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
				//TODO:: IMPLEMENT SOMETHING BECAUSE THIS DOESN't ACTUALLY WORK.. (Add Proper Shader System) @UNFINISHED
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
					
					if (!i.first->unique) //TODO: make this a reallistically accessable flag
					{
						vertex->data = (Empty::VertexBuffer*) malloc(sizeof(*i.first));
						memcpy(vertex->data, i.first, sizeof(Empty::VertexBuffer));


						vertex->data->data = malloc(vertex->data->size);
						memcpy(((void*)vertex->data->data), i.first->data, vertex->data->size);


					}
					else
					{
						vertex->data = i.first;
					}
					//i.first->
					
				
					vertex->bufferData();
					tm->registerVertexBuffer(i.second, vertex);


				}
			}

			return tm;

		}
    }
}
