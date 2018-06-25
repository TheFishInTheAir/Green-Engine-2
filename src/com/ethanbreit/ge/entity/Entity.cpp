#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>


namespace ge
{
	bool Entity::isDestroyed()
	{
		return destroyed;
	}

	void Entity::destroy()
	{
		//TODO: IMPLEMENT
		for(auto tag : tags)
		{
			tag.entities.remove(id);
		}
	}
}
