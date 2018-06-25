#pragma once
#include <cstdint>
#include <unordered_map>

namespace ge
{
	struct EntityTag;

	struct Entity
	{
		uint32_t id;

		std::string name;

		std::vector<EntityTag> tags;
		std::unordered_map<std::string, uint32_t> components;

		bool isDestroyed();
		void destroy();

	private:
		bool destroyed = false;
	};

}
