#include <ge/default_geom/Cube.h>

namespace ge
{
	namespace dgeo
	{
		namespace cube
		{
            const ge::Empty::MeshData getMeshData()
			{
				static ge::Empty::MeshData data;

				static bool first = true;
				if(first)
				{
					first = false;
					data.hasIndexBuffer = true;
					data.hasShaderGroup = false;
					ge::Empty::IndexBuffer* i = new ge::Empty::IndexBuffer();
					i->data = &indicies;
					i->memoryType = ge::BufferMemoryType::Static;
					i->length = 36;
					i->size = sizeof(int) * 36;

					ge::Empty::VertexBuffer* v = new ge::Empty::VertexBuffer();
					v->data = &vertecies;
					v->attributeId = VERTEX_POSITION;
					v->dataType = DataType::Float;
					v->memoryType = BufferMemoryType::Static;
					v->normalized = false;
					v->size = 3 * 36 * sizeof(float);
					v->offset = 0;
					v->sizePerAttrib = 3;


					data.indexBuffer = i;
					data.vertexBuffers.push_back({ v, "vertex" });

				}
				return data;

			}

		}
    }
}