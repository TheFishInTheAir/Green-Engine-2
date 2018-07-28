#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/runtime/RuntimeManager.h>
namespace ge
{
    DefaultComponentBatch::DefaultComponentBatch()
    {
        ge_REGISTER_RUNTIME_HANDLER
     
        setBatchType("DefaultComponentBatch");
        
        RuntimeManager::getRuntime(RUNTIME_MAIN)->getGroup(UPDATE)->
            ge_RUNTIME_GROUP_INSERT_HEAP(this);
        
    }
    
    void DefaultComponentBatch::cycle()
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
    
    void DefaultComponentBatch::softInsert(Component* cmp)
    {
        actionBuffer.push({INSERT, cmp});
    }
    
    void DefaultComponentBatch::softRemove(Component* cmp)
    {
        actionBuffer.push({ REMOVE, cmp });
    }
    
    void DefaultComponentBatch::softRemove(uint32_t i)
    {
        Log::critErr("Unimplemented Function 'DefaultComponentBatch::softRemove(uint32_t)'");
        //TODO: Implement
//        actionBuffer.push({ REMOVE, cmp });
    }
    
    uint32_t DefaultComponentBatch::hardInsert(Component* cmp)
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
    
    void DefaultComponentBatch::hardRemove(uint32_t i)
    {
        components.at(i)->setBatch(nullptr);
        components.at(i) = nullptr;
        emptyIndicies.push(i);
    }
    
    void DefaultComponentBatch::performPendingActions()
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
