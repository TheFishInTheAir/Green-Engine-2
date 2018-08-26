#include <ge/entity/component/components/PointLightComponent.h>
#include <ge/entity/component/batches/PipelineComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/engine/scene/Scene.h>
#include <ge/debug/DebugBox.h>
namespace ge
{


    Component* _constructor_PointLightComponent(Entity* ent)
    {
        return new PointLightComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook PointLightComponent::_hook("PointLightComponent", _constructor_PointLightComponent);


    PointLightComponent::PointLightComponent(Entity* e) : Component(e)
    {
        addPublicVar("Colour",  {DataType::COLOUR3, &light.colour});
        addPublicVar("Ambient",  {DataType::FLOAT, &light.ambient});

        addPublicVar("Constant Attenuation",  {DataType::FLOAT, &light.constant});
        addPublicVar("Linear Attenuation",  {DataType::FLOAT, &light.linear});
        addPublicVar("Quadratic Attenuation",  {DataType::FLOAT, &light.quadratic});
        addPublicVar("Debug Box",  {DataType::BOOL, &debugBox});
        light.colour = glm::vec3(1);
        Scene::currentScene->pointLights.push_back(&light); //NOTE: a little hacky  also this must be run after scene creation or the deadly SEGFAULT
    }

    void PointLightComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void PointLightComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("PipelineComponentBatch", getTypeName()))
        {
            PipelineComponentBatch* cmp = new PipelineComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("PipelineComponentBatch", getTypeName())->softInsert(this);
    }

    void PointLightComponent::cycle()
    {
        if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::Render)
            return;
        if(GraphicsCore::ctx->currentPipeline->getCurrentStage()->type!=PipelineDrawType::Default)
            return;
            
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

        light.pos = transformComponent->getPosition(); //Quaternion Direction is always up, NOTE: I might need to fix the Camera component now considering the quaternion's default direction is up

        if(debugBox)
            Debug::DebugBox::draw(light.colour, transformComponent->getPosition(), glm::vec3(0.2f));

    }



    void PointLightComponent::destroy()
    {
        Scene::currentScene->pointLights.remove(&light);//TODO: make work
    }

    std::string PointLightComponent::getTypeName()
    {
        return "PointLightComponent";
    }
}
