#include <ge/entity/component/components/SpotLightShadowComponent.h>
#include <ge/entity/component/batches/PipelineComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/loader/LoadShader.h>
#include <ge/engine/scene/Scene.h>
#include <ge/debug/DebugBox.h>
namespace ge
{


    Component* _constructor_SpotLightShadowComponent(Entity* ent)
    {
        return new SpotLightShadowComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook SpotLightShadowComponent::_hook("SpotLightShadowComponent", _constructor_SpotLightShadowComponent);


    SpotLightShadowComponent::SpotLightShadowComponent(Entity* e) : Component(e)
    {
        cam = new Camera();
        shadow = new AngleShadow();

        addPublicVar("Should Update", {DataType::BOOL, &shouldUpdate});
        addPublicVar("Shadow Res",  {DataType::INT, &shadowRes});
        addPublicVar("Near Plane",  {DataType::FLOAT, &(cam->nearCull)});
        addPublicVar("Far Plane",  {DataType::FLOAT, &(cam->farCull)});

        ps = new PipelineStage();
        ps->overrideCamera = true;
        ps->cam = cam;
        GraphicsCore::ctx->currentPipeline->stages.push_front(ps); //temp add sorting


        static ShaderGroup* sg;
        static bool shouldInit = true;
        

        if(shouldInit)
        {
            shouldInit = false;
            
            ShaderLoader::loadShader("engine/defaults/common/depth.gesm", &sg);
        }

        ps->overrideShader = true;
        ps->overrideSG = sg;
    }

    void SpotLightShadowComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }

    void SpotLightShadowComponent::insertToDefaultBatch() //TODO: add ingroup priorities
    {

        if(!ComponentManager::containsComponentBatch("PipelineComponentBatch", getTypeName()))
        {
            PipelineComponentBatch* cmp = new PipelineComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("PipelineComponentBatch", getTypeName())->softInsert(this);
    }

    void SpotLightShadowComponent::genFB()
    {
        int realShadowRes = pow(2, shadowRes);

        if(fb!=nullptr)
        {//REMOVE OLD
            fb->deepDestroy();

            free (tex);
            free (fb);
        }
        else
        {
            Scene::currentScene->shadows.push_back(shadow); // pretty much an init for this but in a weird place :)
            int i = 0;
            for(auto s : Scene::currentScene->shadows)
            {
                if(s==shadow)
                {
                    spotLightComponent->light.shadowLoc = i;
                    break;
                }
                i++;
            }
        }
        GraphicsCore::ctx->textureFactory->genFramebuffer(realShadowRes, realShadowRes, &fb);
        fb->generateTextureAttachment(FrameBufferAttachment::Depth24);
        //fb->generateBufferAttachment(FrameBufferAttachment::Depth24_Stencil8);
        tex = fb->getTextureAttachment(FrameBufferAttachment::Depth24);
        shadow->shadowMap = tex;
        ps->framebuffer = fb;

    }



    void SpotLightShadowComponent::cycle()
    {
        if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::PostRender && shouldUpdate)
        {
            shouldUpdate = false;
            genFB();
        }

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
                Log::wrn("SpotLightShadowComponent", "A SpotLightShadowComponent requires a TransformComponent.");
                return;
            }
        }
        if(spotLightComponent == nullptr)
        {

            if(ent->components.count("SpotLightComponent"))
            {
                spotLightComponent = (SpotLightComponent*) ent->components.at("SpotLightComponent");
            }
            else
            {
                Log::wrn("SpotLightShadowComponent", "A SpotLightShadowComponent requires a SpotLightComponent.");
                return;
            }
        }

        cam->pos = spotLightComponent->light.pos;
        cam->dir = cam->pos + spotLightComponent->light.dir;
        cam->aspectRatio = 1;
        cam->fov = glm::radians(spotLightComponent->light.angle+spotLightComponent->light.outerAngle+15); //15 is because.... 
        cam->update();
        shadow->lightSpace = cam->vp;
    }



    void SpotLightShadowComponent::destroy()
    {
    }

    std::string SpotLightShadowComponent::getTypeName()
    {
        return "SpotLightShadowComponent";
    }
}
