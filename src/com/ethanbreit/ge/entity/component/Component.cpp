#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentBatch.h>
#include <ge/entity/Entity.h>

namespace ge
{
    Component::Component(Entity* ent)
    {
        this->ent = ent;
    }
    
    void Component::setBatch(ge::ComponentBatch* batch)
    {
        currentBatch = batch;
    }
    
    ComponentBatch* Component::getBatch()
    {
        return currentBatch;
    }
    
    Entity* Component::getEntity()
    {
        return ent;
    }
}
