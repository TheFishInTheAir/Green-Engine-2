#include <ge/entity/EntityManager.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <ge/console/Log.h>
#include <vector>
#include <queue>

namespace ge
{
    namespace EntityManager
    {
		std::unordered_map<std::string, EntityTag*>* tags;

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

				tags = new std::unordered_map<std::string, EntityTag*>();
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
                ent->id = i;
                return i;
            }

            entities->push_back(ent);
            ent->id = entities->size()-1;

            return entities->size()-1;

        }

        void removeEntity(uint32_t id)
        {
            _init();

            entities->at(id) = nullptr;
            emptyIndexs->push(id);
        }

        void removeEntity(Entity* ent)
        {
			_init();

            removeEntity(ent->id);
        }

		std::vector<Entity*>* getAllEntities()
		{
			_init();

			return entities;
		}


        void regsiterTag(EntityTag* tag)
        {
			_init();

			tags->insert({tag->name, tag});
        }

        void removeTag(EntityTag* tag)
        {
			_init();

			removeTag(tag->name);
        }

        void removeTag(std::string str)
        {
			_init();

			tags->erase(str);
        }

		bool tagExists(std::string str)
		{
			_init();

			return tags->count(str) != 0;
		}

		EntityTag*	getTag(std::string str)
		{
			_init();

			return tags->at(str);
		}
		EntityTag*	getOrCreateTag(std::string str)
		{
			_init();

			EntityTag* tag;
			if (!tagExists(str))
				regsiterTag(tag = new EntityTag(str));
			else
				tag = getTag(str);
			return tag;
		}

    }
}
