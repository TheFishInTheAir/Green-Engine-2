#pragma once
#include <vector>
#include <ge/graphics/types/VertexBuffer.h>
#include <ge/graphics/types/Texture.h>
#include <ge/graphics/types/ShaderGroup.h>
#include <ge/graphics/types/IndexBuffer.h>

namespace ge
{
	namespace Empty
	{
		struct MeshData
		{
			typedef std::pair<VertexBuffer*, std::string> vertBuf;
			typedef std::pair<Texture*, unsigned int> tex;


			std::vector<vertBuf>	vertexBuffers;
			std::vector<tex>		Textures;
			ShaderGroup* shaderGroup;
			IndexBuffer* indexBuffer;

			bool hasShaderGroup = false;
			bool hasIndexBuffer = false;

		};
	}
}
