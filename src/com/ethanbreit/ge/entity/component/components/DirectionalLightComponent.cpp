#include <ge/entity/component/components/DirectionalLightComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/engine/scene/Scene.h>
#include <ge/debug/DebugBox.h>
namespace ge
{


    Component* _constructor_DirectionalLightComponent(Entity* ent)
    {
        return new DirectionalLightComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook DirectionalLightComponent::_hook("DirectionalLightComponent", _constructor_DirectionalLightComponent);


    DirectionalLightComponent::DirectionalLightComponent(Entity* e) : Component(e)
    {
        addPublicVar("Colour",  {DataType::COLOUR3, &light.colour});
        addPublicVar("Ambient",  {DataType::FLOAT, &light.ambient});
        addPublicVar("Debug Box",  {DataType::BOOL, &debugBox});
        light.colour = glm::vec3(1);
        light.ambient = 0.2;
        Scene::currentScene->directionalLights.push_back(&light); //NOTE: a little hacky  also this must be run after scene creation or the deadly SEGFAULT
        lightIter = Scene::currentScene->directionalLights.size()-1;
    }

    void DirectionalLightComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void DirectionalLightComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void DirectionalLightComponent::cycle()
    {
        if(transformComponent == nullptr)
        {

            if(ent->components.count("TransformComponent"))
            {
                transformComponent = (TransformComponent*) ent->components.at("TransformComponent");
                
                Log::dbg("good test");
                Log::dbg("good test2");

            }
            else
            {
                Log::wrn("LightComponent", "A LightComponent requires a TransformComponent.");
                return;
            }
        }

        light.dir = glm::vec3(0,1,0) * transformComponent->getRotation(); //Quaternion Direction is always up, NOTE: I might need to fix the Camera component now considering the quaternion's default direction is up

        if(debugBox)
            Debug::DebugBox::draw(light.colour, transformComponent->getPosition(), glm::vec3(0.2f));

    }



    void DirectionalLightComponent::destroy()
    {
        Scene::currentScene->directionalLights.erase(Scene::currentScene->directionalLights.begin()+lightIter);//TODO: make work
    }

    std::string DirectionalLightComponent::getTypeName()
    {
        return "DirectionalLightComponent";
    }
}
