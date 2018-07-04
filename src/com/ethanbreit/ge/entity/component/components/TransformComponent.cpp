#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>


namespace ge
{
    void TransformComponent::defaultInit()
    {
        
    }
    
    void TransformComponent::insertToDefaultBatch()
    {
        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            ComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);

    }
    
    void TransformComponent::destroy()
    {
        
    }
    
    void TransformComponent::cycle()
    {
        if(shouldUpdate)
        {
            shouldUpdate=false;
            model = glm::mat4(1); // I think this is an identity matrix
            glm::translate(model, pos);
            model *= glm::toMat4(rot);
            model *= glm::scale(model, scale);
        }
    }
    
    void TransformComponent::setPosition(glm::vec3 v)
    {
        shouldUpdate = true;
        pos = v;
    }
    
    void TransformComponent::setScale(glm::vec3 v)
    {
        shouldUpdate = true;
        scale = v;
    }
    
    void TransformComponent::setRotation(glm::quat q)
    {
        shouldUpdate = true;
        rot = q;
    }
    
    glm::vec3 TransformComponent::getPosition()
    {
        return pos;
    }
    glm::vec3 TransformComponent::getScale()
    {
        return scale;
    }
    glm::quat TransformComponent::getRotation()
    {
        return rot;
    }
    
    glm::mat4 TransformComponent::getModel()
    {
        return model;
    }

    std::string TransformComponent::getTypeName()
    {
        return "TransformComponent";
    }
}
