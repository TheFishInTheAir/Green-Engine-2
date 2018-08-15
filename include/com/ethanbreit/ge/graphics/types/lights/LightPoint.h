#pragma once
#include <glm/glm.hpp>
#include <ge/graphics/meshes/TriangleMesh.h>

namespace ge
{
	struct LightPoint
	{
		glm::vec3 pos;
		glm::vec3 colour;

		float constant;
		float linear;
		float quadratic;

		void pushUnifValues(TriangleMesh* mesh, std::string prefix);

	};
}
