#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentBatch.h>
#include <ge/console/Log.h>
#include <stdlib.h>

namespace ge
{
	const std::string LOG_TAG = "Entity";

	bool Entity::isDestroyed()
	{
		return destroyed;
	}
    
    void Entity::insertComponent(Component* cmp)
    {
		if(components.count(cmp->getTypeName())>=1)
		{
			Log::err(LOG_TAG, "Cannot insert Component with type '"+cmp->getTypeName()+"', one already exists in entity.");
			return;
		}
        components.insert({cmp->getTypeName(), cmp});
    }

	void Entity::removeComponent(std::string type)
	{
		if(components.count(type)==0)
		{
			Log::err(LOG_TAG, "Cannot remove Component with type '"+type+"', none exist in entity.");
			return;
		}
		components.erase(type);
	}
    
	void Entity::registerToTag(EntityTag* tag)
	{
		tag->entities.push_back(this);
		tags.push_back(tag);
	}

    
	void Entity::destroy()
	{
		//TODO: IMPLEMENT
		for(auto tag : tags)
		{
			tag->entities.remove(this);
		}
        
        for(auto cmp : components)
        {
            cmp.second->destroy();
            cmp.second->getBatch()->hardRemove(cmp.second->batchId);
			free(cmp.second);
        }
	}
}
