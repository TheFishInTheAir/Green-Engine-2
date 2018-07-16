#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentBatch.h>
namespace ge
{
	bool Entity::isDestroyed()
	{
		return destroyed;
	}
    
    void Entity::insertComponent(Component* cmp)
    {
        components.insert({cmp->getTypeName(), cmp});
    }
    

    
	void Entity::destroy()
	{
		//TODO: IMPLEMENT
		for(auto tag : tags)
		{
			tag->entities.remove(id);
		}
        
        for(auto cmp : components)
        {
            cmp.second->destroy();
            cmp.second->getBatch()->softRemove(cmp.second->batchId);
        }
	}
}
