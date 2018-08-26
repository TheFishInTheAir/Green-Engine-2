#include <ge/entity/component/batches/EngineControllerComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/runtime/RuntimeManager.h>
#include <ge/graphics/GraphicsCore.h>

namespace ge
{
    EngineControllerComponentBatch::EngineControllerComponentBatch()
    {
        ge_REGISTER_RUNTIME_HANDLER
        
        setBatchType("EngineControllerComponentBatch");
        
        RuntimeManager::getRuntime(RUNTIME_MAIN)->getGroup(PIPELINE_ROUTER_RG)->
        ge_RUNTIME_GROUP_INSERT_HEAP(this);
        
    }
    
    void EngineControllerComponentBatch::cycle()
    {
        performPendingActions();

        if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::PostRender)
            return;

        for(Component* component : components)
        {
            if(component!=nullptr)
                component->cycle();
        }
    }
    
    void EngineControllerComponentBatch::softInsert(Component* cmp)
    {
        actionBuffer.push({INSERT, cmp});
    }
    
    void EngineControllerComponentBatch::softRemove(Component* cmp)
    {
        actionBuffer.push({ REMOVE, cmp });
    }
    
    void EngineControllerComponentBatch::softRemove(uint32_t i)
    {
        Log::critErr("Unimplemented Function 'EngineControllerComponentBatch::softRemove(uint32_t)'");
        //TODO: Implement
        //        actionBuffer.push({ REMOVE, cmp });
    }
    
    uint32_t EngineControllerComponentBatch::hardInsert(Component* cmp)
    {
        cmp->setBatch(this);
        
        if(emptyIndicies.empty())
        {
            components.push_back(cmp); //if full
            cmp->batchId = components.size()-1;
            return cmp->batchId;
        }
        else
        {
            uint32_t i = emptyIndicies.front();
            components.at(i) = cmp;
            return i;
        }
    }
    
    void EngineControllerComponentBatch::hardRemove(uint32_t i)
    {
        components.at(i)->setBatch(nullptr);
        components.at(i) = nullptr;
        emptyIndicies.push(i);
    }
    
    void EngineControllerComponentBatch::performPendingActions()
    {
        while (actionBuffer.size()!=0)
        {
            switch(actionBuffer.front().first)
            {
                case INSERT:
                    hardInsert(actionBuffer.front().second);
                    break;
                case REMOVE:
                    hardRemove(actionBuffer.front().second->batchId);
                    break;
            }
            actionBuffer.pop();
            
        }
    }
    
}
