#pragma once
#include <glm/glm.hpp>
#include <ge/graphics/meshes/TriangleMesh.h>
namespace ge
{
	struct LightDirectional
	{
		glm::vec3 colour;
		glm::vec3 dir;
		float ambient;

		void pushUnifValues(TriangleMesh* mesh, std::string prefix);
	};
}
