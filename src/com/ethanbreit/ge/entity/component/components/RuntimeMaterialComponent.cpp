#include <ge/entity/component/components/RuntimeMaterialComponent.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/PipelineComponentBatch.h>
#include <ge/entity/Entity.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/graphics/pipeline/Pipeline.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/graphics/Camera.h>
#include <ge/engine/scene/Scene.h>
#include <ge/graphics/UniformAutomator.h>

namespace ge
{

    Component* _constructor_RuntimeMaterialComponent(Entity* ent)
    {
        return new RuntimeMaterialComponent(ent);
    }

    ComponentConstructorRegistry::StartupHook RuntimeMaterialComponent::_hook("RuntimeMaterialComponent", _constructor_RuntimeMaterialComponent);


    RuntimeMaterialComponent::RuntimeMaterialComponent(Entity* ent) : ge::Component(ent)
    {
        mat = new Material();
        mat->name = "undefined";
        mat->url = "RUNTIME";
        addPublicVar("Name", {DataType::STRING, &(mat->name)});
        addPublicVar("PushToScene",  {DataType::BOOL, &shouldPut});
        addPublicVar("Name", {DataType::STRING, &(mat->name)});
    }

    
	void RuntimeMaterialComponent::defaultInit()
	{
		//TODO: Make this good
	}
    
    void RuntimeMaterialComponent::insertToDefaultBatch()
    {
            //TODO: make a batched rendering component
        if(!ComponentManager::containsComponentBatch("PipelineComponentBatch", getTypeName()))
        {
            ComponentBatch* cmp = new PipelineComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("PipelineComponentBatch", getTypeName())->softInsert(this);
    }
    
    void RuntimeMaterialComponent::cycle()
    {
        if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::PostRender)
            return;
        if(shouldPut==true)
        {
            shouldPut = false;
            Scene::currentScene->materials[mat->name] = *mat;
        }
    }


    void RuntimeMaterialComponent::destroy()
    {
        delete mat;
    }
    
    std::string RuntimeMaterialComponent::getTypeName()
    {
        return "RuntimeMaterialComponent";
    }
}


