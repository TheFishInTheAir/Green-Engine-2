#pragma once
#include "glm/glm.hpp"
#include <ge/graphics/enums/LightType.h>

namespace ge
{
	//NOTE: DEPRECATED TODO: REMOVE
	struct Light
	{

		glm::vec3 pos;
		glm::vec3 dir;
		float angle;

		glm::vec3 colour;

		LightType::type LightType;
		
		/* //Global:
				
		glm::vec3 pos;

		glm::vec3 colour;
		float intensity; ///Implement soon

		virtual LightType::type getLightType() = 0;

		//TODO: add more parameters. @UNFINISHED */

	};
}
