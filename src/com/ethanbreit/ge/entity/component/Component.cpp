#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/console/Log.h>
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
    void Component::addPublicVar(std::string name, std::pair<ge::DataType::type, void*> data)
    {
        publicVars.insert({name, data});
        publicVarsDisplayOrder.push_back(name);
    }

}
