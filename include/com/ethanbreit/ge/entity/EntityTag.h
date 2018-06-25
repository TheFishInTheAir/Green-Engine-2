#pragma once
#include <string>
#include <list>

namespace ge
{
	struct EntityTag
	{
		std::string name;

		std::list<uint32_t> entities;

		//TODO: add insertion and removal
	};
}
