#include <ge/entity/component/components/SpotLightComponent.h>
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


    Component* _constructor_SpotLightComponent(Entity* ent)
    {
        return new SpotLightComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook SpotLightComponent::_hook("SpotLightComponent", _constructor_SpotLightComponent);


    SpotLightComponent::SpotLightComponent(Entity* e) : Component(e)
    {
        addPublicVar("Colour",  {DataType::COLOUR3, &light.colour});
        addPublicVar("Ambient",  {DataType::FLOAT, &light.ambient});
        addPublicVar("Init Shadow",  {DataType::BOOL, &castShadows});
        addPublicVar("Shadow Res",  {DataType::FLOAT, &shadowRes});
        addPublicVar("Angle",  {DataType::FLOAT, &light.angle});
        addPublicVar("Outer Angle",  {DataType::FLOAT, &light.outerAngle});
        addPublicVar("Debug Box",  {DataType::BOOL, &debugBox});
        light.colour = glm::vec3(1);
        Scene::currentScene->spotLights.push_back(&light); //NOTE: a little hacky  also this must be run after scene creation or the deadly SEGFAULT 
    }

    void SpotLightComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void SpotLightComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("PipelineComponentBatch", getTypeName()))
        {
            PipelineComponentBatch* cmp = new PipelineComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("PipelineComponentBatch", getTypeName())->softInsert(this);
    }

    void SpotLightComponent::shadowCycle()
    {
        if(hasInitShadows)
        {
            hasInitShadows = false;
            
            int realShadowRes = pow(2, shadowRes);
            cam = new Camera();

            PipelineStage* ps = new PipelineStage();
            ps->overrideCamera = true;
            ps->cam = cam;
            FrameBuffer* fb;
            GraphicsCore::ctx->textureFactory->genFramebuffer(realShadowRes, realShadowRes, &fb);
            fb->generateTextureAttachment(FrameBufferAttachment::Depth);
            ps->framebuffer = fb;
            GraphicsCore::ctx->currentPipeline->stages.push_front(ps);//temp add sorting
        }

        cam->pos = light.pos;
        cam->dir = light.dir;
        cam->aspectRatio = 1;
        cam->fov = glm::radians(light.angle);
        cam->update();
    }

    void SpotLightComponent::cycle()
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
            }
            else
            {
                Log::wrn("LightComponent", "A LightComponent requires a TransformComponent.");
                return;
            }
        }

        light.pos = transformComponent->getPosition(); //Quaternion Direction is always up, NOTE: I might need to fix the Camera component now considering the quaternion's default direction is up
        light.dir = glm::vec3(0,1,0) * transformComponent->getRotation();
        light.dir.y = -light.dir.y; //NOTE: I DON'T KNOW WHY THIS FIXES THIS

        if(castShadows)
            shadowCycle();
        
        if(debugBox)
        {
            Debug::DebugBox::draw(light.colour, transformComponent->getPosition(), glm::vec3(0.2f));
            Debug::DebugBox::draw(light.colour*1.2f, transformComponent->getPosition(), glm::vec3(0.1f, 0.5f, 0.1f), transformComponent->getRotation());
          
        }

    }



    void SpotLightComponent::destroy()
    {
        Scene::currentScene->spotLights.remove(&light);//TODO: make work
    }

    std::string SpotLightComponent::getTypeName()
    {
        return "SpotLightComponent";
    }
}
