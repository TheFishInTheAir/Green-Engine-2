#pragma once
#include <ge/entity/Entity.h>
#include <json/json.hpp>
#include <list>
namespace ge
{
    namespace EntityDeserializer
    {
        std::list<Entity*> deserializeJson(nlohmann::json);

        void activateEntities(std::list<Entity*>);
    }
}