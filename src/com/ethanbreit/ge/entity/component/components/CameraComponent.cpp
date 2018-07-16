#include <ge/entity/component/components/CameraComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
namespace ge
{
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
        camera.update(); //we only use this to recalculate the projection
        //NOTE: INNEFICIENT DOING AN UPDATE THEN RECALCULATING VIEW MATRIX

        camera.view = glm::mat4(1);
        camera.view *= glm::translate(glm::mat4(1), transformComponent->getPosition());
        camera.view *= glm::toMat4(transformComponent->getRotation());

        camera.view = glm::inverse(camera.view);
        camera.vp = camera.proj*camera.view;
    }



    void CameraComponent::destroy()
    {

    }

    std::string CameraComponent::getTypeName()
    {
        return "CameraComponent";
    }
}
