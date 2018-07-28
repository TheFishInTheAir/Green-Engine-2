#include <ge/entity/component/components/LightComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/engine/scene/Scene.h>
namespace ge
{


    Component* _constructor_LightComponent(Entity* ent)
    {
        return new LightComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook LightComponent::_hook("LightComponent", _constructor_LightComponent);


    LightComponent::LightComponent(Entity* e) : Component(e)
    {
        addPublicVar("Colour",  {DataType::COLOUR3, &light.colour});
        addPublicVar("Angle",  {DataType::FLOAT, &light.angle});
        addPublicVar("Light Type",  {DataType::INT, &light.light_type});

        light.light_type = (LightType::type) LIGHT_DIRECTIONAL;
        light.colour = glm::vec3(1);

        Scene::currentScene->lights.push_back(&light); //NOTE: a little hacky  also this must be run after scene creation or the deadly SEGFAULT
        lightIter = Scene::currentScene->lights.size()-1;
    }

    void LightComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void LightComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            DefaultComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }

        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }

    void LightComponent::cycle()
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

        light.pos = transformComponent->getPosition();
        light.dir = glm::vec3(0,1,0) * transformComponent->getRotation(); //Quaternion Direction is always up, NOTE: I might need to fix the Camera component now considering the quaternion's default direction is up

    }



    void LightComponent::destroy()
    {
        Scene::currentScene->lights.erase(Scene::currentScene->lights.begin()+lightIter);//TODO: make work
    }

    std::string LightComponent::getTypeName()
    {
        return "LightComponent";
    }
}
