#include <ge/entity/EntityManager.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <vector>
#include <queue>

namespace ge
{
    namespace EntityManager
    {

        std::vector<Entity*>* entities;
        std::queue<uint32_t>* emptyIndexs;

        void _init()
        {
            static bool initialised = false;

            if(!initialised)
            {
                initialised = true;

                entities = new std::vector<Entity*>();
                emptyIndexs = new std::queue<uint32_t>();
            }
        }

        uint32_t registerEntity(Entity* ent)
        {
            _init();

            if(!emptyIndexs->empty())
            {
                uint32_t i = emptyIndexs->front();

                entities->at(i) = ent;

                emptyIndexs->pop();
                return i;
            }

            entities->push_back(ent);
            return entities->size()-1;

        }

        void removeEntity(uint32_t id)
        {
            _init();

            entities->at(id) = nullptr;

        }

        void removeEntity(Entity* ent)
        {

        }


        void regsiterTag(EntityTag* tag)
        {

        }

        void removeTag(EntityTag* tag)
        {

        }

        void removeTag(std::string str)
        {

        }

    }
}
