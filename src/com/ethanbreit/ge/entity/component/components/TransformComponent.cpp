#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/graphics/Camera.h>
#include <ge/console/Log.h>


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
    
    void TransformComponent::reCalc()
    {
        shouldUpdate=true;
    }
    
    void TransformComponent::cycle()
    {
        if(shouldUpdate)
        {
            shouldUpdate=false;
            model = glm::translate(glm::mat4(1), pos);
            model *= glm::toMat4(rot);
            model = glm::scale(model, scale);

            if(Camera::displayCamera==nullptr)
                mvp = model;
            else
                mvp = Camera::displayCamera->vp * model;
        }
    }
    
    void TransformComponent::setPosition(glm::vec3 v)
    {
        pos = v;
        shouldUpdate = true;
    }
    
    void TransformComponent::setScale(glm::vec3 v)
    {
        scale = v;
        shouldUpdate = true;
    }
    
    void TransformComponent::setRotation(glm::quat q)
    {
        rot = q;
        shouldUpdate = true;
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
    
    glm::mat4 TransformComponent::getMVP()
    {
        if(Camera::displayCamera==nullptr)
            mvp = model;
        else
            mvp = Camera::displayCamera->vp * model;
        return mvp;
    }

    
    //All below are deprecated
    glm::vec3* TransformComponent::getUPosition()
    {
        return &pos;
    }
    glm::vec3* TransformComponent::getUScale()
    {
        return &scale;
    }
    glm::quat* TransformComponent::getURotation()
    {
        return &rot;
    }
    
    glm::mat4* TransformComponent::getUModel()
    {
        return &model;
    }
    
    glm::mat4* TransformComponent::getUMVP()
    {
        if(Camera::displayCamera==nullptr)
            mvp = model;
        else
            mvp = Camera::displayCamera->vp * model;
        return &mvp;
    }
    
    std::string TransformComponent::getTypeName()
    {
        return "TransformComponent";
    }
}
