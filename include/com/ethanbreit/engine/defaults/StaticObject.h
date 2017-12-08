#pragma once
#include "engine/types/ModelMat.h"
#include "engine/empty_types/EmptyStaticObject.h"
//#include "engine/scene/Scene.h"

#include "graphics/meshes/TriangleMesh.h"
#include <memory>

namespace ge
{
	struct Scene;

	struct StaticObject
	{

		StaticObject(Empty::StaticObject);

		StaticObject(Empty::StaticObject, Scene);

		ModelMat model;

		TriangleMesh* mesh;

		ShaderGroup* shaders;

		std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> roughness;
	};
}
