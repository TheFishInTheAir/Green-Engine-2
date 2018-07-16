#pragma once
#include <ge/graphics/types/Window.h>
#include "ge/graphics/enums/GraphicsApi.h"

namespace ge
{

	struct SimpleEngineCreationInfo
	{
		GraphicsApi::type     g_api;
		WindowConstructorInfo wci;
        glm::vec3 clear_colour = glm::vec3(1); //TODO: maybe put in Window Constructor Info

		std::vector<Runtime*> runtimes;
		std::vector<std::pair<RuntimeGroup*, std::string>> runtime_groups;

	};

	void simpleStart(SimpleEngineCreationInfo);
}
