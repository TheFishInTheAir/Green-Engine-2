#include <ge/entity/component/components/CameraComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
namespace ge
{
    Component* _constructor_CameraComponent(Entity* ent)
    {
        return new CameraComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook CameraComponent::_hook("CameraComponent", _constructor_CameraComponent);

    CameraComponent::CameraComponent(Entity* e) : Component(e)
    {
        addPublicVar("FOV", {DataType::FLOAT, &camera.fov});
        addPublicVar("Near Cull Plane", {DataType::FLOAT, &camera.nearCull});
        addPublicVar("Far Cull Plane", {DataType::FLOAT, &camera.farCull});
		addPublicVar("Make Display", {DataType::BOOL, &shouldMakeDisplay});
    }

    void CameraComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void CameraComponent::makeDisplay()
    {
        Camera::displayCamera = &camera;
    }

    void CameraComponent::insertToDefaultBatch()
    {

        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void CameraComponent::cycle()
    {
        if(!hasTransformComponent)
        {

            if(ent->components.count("TransformComponent"))
            {
                transformComponent = (TransformComponent*) ent->components.at("TransformComponent");
                hasTransformComponent = true;
            }

            Log::wrn("CameraComponent", "A CameraComponent requires a TransformComponent.");
            return;
        }
		if (shouldMakeDisplay)
		{
			shouldMakeDisplay = false;
			makeDisplay();
		}

        camera.update(); //we only use this to recalculate the projection
        //NOTE: INNEFICIENT DOING AN UPDATE THEN RECALCULATING VIEW MATRIX

        camera.dir = glm::vec3(0,0,-1) * transformComponent->getRotation(); //transform quaternion into vector
        camera.up  = glm::vec3(0,1,0) * transformComponent->getRotation(); //transform quaternion into vector
        camera.pos = transformComponent->getPosition();
        
        camera.view = glm::mat4(1);
        camera.view *= glm::translate(glm::mat4(1), transformComponent->getPosition());
        camera.view *= glm::toMat4(transformComponent->getRotation());

        camera.view = glm::inverse(camera.view);
        camera.vp = camera.proj*camera.view;
    }



    void CameraComponent::destroy()
    {
        if(Camera::displayCamera==&camera)
            Camera::displayCamera=nullptr;
    }

    std::string CameraComponent::getTypeName()
    {
        return "CameraComponent";
    }
}
