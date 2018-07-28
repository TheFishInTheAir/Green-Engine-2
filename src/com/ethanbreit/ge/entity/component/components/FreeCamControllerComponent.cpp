#include <ge/entity/component/components/FreeCamControllerComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/input/MouseHandler.h>
#include <ge/util/RotationUtil.h>
#include <math.h>
namespace ge
{

    Component* _constructor_FreeCamControllerComponent(Entity* ent)
    {
        return new FreeCamControllerComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook FreeCamControllerComponent::_hook("FreeCamControllerComponent", _constructor_FreeCamControllerComponent);

    
    
    
    FreeCamControllerComponent::FreeCamControllerComponent(Entity* e) : Component(e)
    {
        addPublicVar("Look Speed",  {DataType::FLOAT, &lookSpeed});
        addPublicVar("Move Speed",  {DataType::FLOAT, &moveSpeed});    
    }


    void FreeCamControllerComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void FreeCamControllerComponent::insertToDefaultBatch() //TODO: add in batch prioreties
    {
        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void FreeCamControllerComponent::cycle()
    {
        if(!hasTransformComponent)
        {

            if(ent->components.count("TransformComponent"))
            {
                tComp = (TransformComponent*) ent->components.at("TransformComponent");
                hasTransformComponent = true;
            }

            Log::wrn("FreeCamController", "A FreeCamController requires a TransformComponent.");
            return;
        }

        Runtime* r = RuntimeManager::getRuntime(RUNTIME_MAIN);

        
        float localMoveSpeed = (moveSpeed/10000) * r->getLastDelta();
        
        double x = 0, y = 0;
        
        MouseHandler::getMousePos(&x, &y);
        horizontalAngle += float(-x)*(lookSpeed/10000);
        verticalAngle  +=  float(-y)*(lookSpeed/10000);
        MouseHandler::resetMousePos();

        if(verticalAngle>=glm::pi<float>()/2)
            verticalAngle=glm::pi<float>()/2;
        
        if(verticalAngle<=-(glm::pi<float>()/2))
            verticalAngle=-(glm::pi<float>()/2);

        glm::quat q = glm::normalize(glm::quat(glm::vec3(verticalAngle, horizontalAngle, 0))); //pitch, yaw, roll
        tComp->setRotation(q);
        
        glm::vec3 forward = -glm::vec3(glm::sin(horizontalAngle), 0, glm::cos(horizontalAngle));
        glm::vec3 right   = glm::cross(forward, glm::vec3(0,1,0));
        
        
        if(KeyboardHandler::keyDown(KeyboardKey::LSHIFT))
            localMoveSpeed *= sprintModif;

        
        
        if(KeyboardHandler::keyDown(KeyboardKey::W))
            tComp->setPosition(tComp->getPosition()+(forward*localMoveSpeed));
        
        if(KeyboardHandler::keyDown(KeyboardKey::A))
            tComp->setPosition(tComp->getPosition()+(-right*localMoveSpeed));
        
        if(KeyboardHandler::keyDown(KeyboardKey::S))
            tComp->setPosition(tComp->getPosition()+(-forward*localMoveSpeed));
        
        if(KeyboardHandler::keyDown(KeyboardKey::D))
            tComp->setPosition(tComp->getPosition()+(right*localMoveSpeed));
        
        
        if(KeyboardHandler::keyDown(KeyboardKey::SPACE))
            tComp->setPosition(tComp->getPosition()+(glm::vec3(0,1,0)*localMoveSpeed));
        
        if(KeyboardHandler::keyDown(KeyboardKey::LCTRL))
            tComp->setPosition(tComp->getPosition()+(glm::vec3(0,-1,0)*localMoveSpeed));
        
        
        
    }

    void FreeCamControllerComponent::destroy()
    {

    }

    std::string FreeCamControllerComponent::getTypeName()
    {
        return "FreeCamControllerComponent";
    }
}
