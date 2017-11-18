#pragma once
#include "engine/types/ModelMat.h"
#include "engine/empty_types/StaticObject.h"

#include "graphics/meshes/TriangleMesh.h"

namespace ge
{
	struct StaticObject
	{

		StaticObject(Empty::StaticObject);

		
		ModelMat model;

		TriangleMesh* mesh;

		ShaderGroup* shaders;

		Texture* albedo;
		Texture* specular;
		Texture* normals;
		Texture* roughness;
	};
}
