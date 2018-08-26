#include <ge/entity/component/components/EngineControllerComponent.h>
#include <ge/entity/component/batches/EngineControllerComponentBatch.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/console/Log.h>
namespace ge
{
    void EngineControllerComponent::defaultInit()
    {
        //TODO: probably should remove this.
    }
    
    void EngineControllerComponent::insertToDefaultBatch()
    {
        
        if(!ComponentManager::containsComponentBatch("EngineControllerComponentBatch", getTypeName()))
        {
            EngineControllerComponentBatch* cmp = new EngineControllerComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("EngineControllerComponentBatch", getTypeName())->softInsert(this);
    }
    
    void EngineControllerComponent::cycle() //NOTE: is this necessary anymore? I'll keep it here for now but, I don't think I need it anymore.
    { //NOTE: this shouldn't be in a component...
        // static bool first = true;
        // if(first)
        // {
        //     first = false;
        //     ge::GraphicsCore::ctx->window->makeCurrentThread(ge::RuntimeManager::getRuntime(RUNTIME_MAIN));
        // }
        
        //GraphicsCore::ctx->window->swap();
        //GraphicsCore::ctx->window->clear();
    }

    void EngineControllerComponent::destroy()
    {
        
    }
    
    std::string EngineControllerComponent::getTypeName()
    {
        return "EngineControllerComponent";
    }
}
