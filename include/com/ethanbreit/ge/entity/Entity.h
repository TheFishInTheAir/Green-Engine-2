#pragma once
#include <cstdint>
#include <unordered_map>
//#include <map>
#include <string>
#include <vector>


namespace ge
{
	struct EntityTag;
    struct Component;
    
	struct Entity
	{
		uint32_t id;

		std::string name;

		std::vector<EntityTag*> tags;
		std::unordered_map<std::string, Component*> components;
        
        void insertComponent(Component*);
        
		bool isDestroyed();
		void destroy();

	private:
		bool destroyed = false;
	};

}
