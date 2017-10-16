#pragma once
#include "glm/glm.hpp"
#include "graphics/enums/LightType.h"

namespace ge
{
	struct Light
	{
		LightType::type type;
		//Global:
				
		glm::vec3 pos;

		glm::vec3 colour;
		float intensity; ///Implement soon
		
		// Directional, Spot

		glm::vec3 dir;

		//Spot
		float coneAngle;

		//TODO: add more parameters.
	};
}
