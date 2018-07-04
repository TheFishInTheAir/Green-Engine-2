#include <ge/entity/component/ComponentManager.h>
#include <unordered_map>
#include <string>
#include <ge/console/Log.h>


namespace ge
{
    namespace ComponentManager
    {
        std::string LOG_TAG = "ComponentManager";
        
        // <<Component Type, Batch Type>, Component Batch>
        std::unordered_map<std::string, ComponentBatch*> batches;
        
        
        void registerComponentBatch(ComponentBatch* batch)
        {
            batches.insert({batch->getBatchType()+":"+batch->getComponentType(),batch});
            Log::scc(LOG_TAG, "Registered new Batch: "+batch->getBatchType()+":"+batch->getComponentType());
        }
        
        bool containsComponentBatch(std::string batchType, std::string componentType)
        {
            return batches.count(batchType+":"+componentType)>0; 
        }
        
        ComponentBatch* getComponentBatch(std::string batchType, std::string componentType)
        {
            if(batches.count(batchType+":"+componentType))
            {
                return batches.at(batchType+":"+componentType);
            }
            
            Log::err(LOG_TAG, "Could not find Component batch with batch type: '"+batchType+"' and component type: '"+componentType+"'.");
            return nullptr;
        }
    }
}
