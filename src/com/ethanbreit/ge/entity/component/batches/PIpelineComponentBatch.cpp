#include <ge/entity/component/batches/PipelineComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/runtime/RuntimeManager.h>
namespace ge
{
    PipelineComponentBatch::PipelineComponentBatch()
    {
        ge_REGISTER_RUNTIME_HANDLER
     
        setBatchType("PipelineComponentBatch");
        
        //NOTE: this doesn't account for multithreaded applications
        RuntimeManager::getRuntime(RUNTIME_MAIN)->getGroup(PIPELINE_ROUTER_RG)->
            ge_RUNTIME_GROUP_INSERT_HEAP(this);
        
    }
    
    void PipelineComponentBatch::cycle()
    {
        //Log::dbg(componentType+", start");
        performPendingActions();
        
        for(Component* component : components)
        {
            if(component!=nullptr)
                component->cycle();
        }
        //Log::dbg(componentType+", stop");
    }
    
    void PipelineComponentBatch::softInsert(Component* cmp)
    {
        actionBuffer.push({INSERT, cmp});
    }
    
    void PipelineComponentBatch::softRemove(Component* cmp)
    {
        actionBuffer.push({ REMOVE, cmp });
    }
    
    void PipelineComponentBatch::softRemove(uint32_t i)
    {
        Log::critErr("Unimplemented Function 'PipelineComponentBatch::softRemove(uint32_t)'");
        //TODO: Implement
//        actionBuffer.push({ REMOVE, cmp });
    }
    
    uint32_t PipelineComponentBatch::hardInsert(Component* cmp)
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
            emptyIndicies.pop();

            components.at(i) = cmp;
            cmp->batchId = i;
            return i;
        }
    }
    
    void PipelineComponentBatch::hardRemove(uint32_t i)
    {
        components.at(i)->setBatch(nullptr);
        components.at(i) = nullptr;
        emptyIndicies.push(i);
    }
    
    void PipelineComponentBatch::performPendingActions()
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
