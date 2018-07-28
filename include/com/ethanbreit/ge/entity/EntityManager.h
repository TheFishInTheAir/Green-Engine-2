#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace ge
{
    struct Entity;
    struct EntityTag;


    namespace EntityManager
    {
        uint32_t registerEntity(Entity*);
        void     removeEntity(uint32_t);
        void     removeEntity(Entity*);

        void     registerTag(EntityTag*);
        void     removeTag(EntityTag*);
        void     removeTag(std::string);

        std::vector<Entity*>* getAllEntities();
    }
}
