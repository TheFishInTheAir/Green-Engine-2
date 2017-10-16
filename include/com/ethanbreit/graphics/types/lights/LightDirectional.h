#pragma once
#include "Light.h"

namespace ge
{
	struct LightDirectional : Light
	{
		glm::vec3 dir;
	};
}
