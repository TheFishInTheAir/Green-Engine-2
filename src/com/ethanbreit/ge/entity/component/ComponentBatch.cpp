#include <ge/entity/component/ComponentBatch.h>
#include <ge/console/Log.h>

namespace ge
{
    std::string LOG_TAG = "ComponentBatch";
    
    void ComponentBatch::setComponentType(std::string type)
    {
        if(componentType!="UNDEFINED")
            Log::critErr(LOG_TAG, "Component Type Redefinition Attempted.");

        componentType = type;
    }
    
    void ComponentBatch::setBatchType(std::string type)
    {
        if(componentType!="UNDEFINED")
            Log::critErr(LOG_TAG, "Batch Type Redefinition Attempted.");
        batchType = type;
    }
    
    std::string ComponentBatch::getBatchType()
    {
        if(batchType=="UNDEFINED")
            Log::wrn(LOG_TAG, "Batch type is not set.");
        return batchType;
    }
    
    std::string ComponentBatch::getComponentType()
    {
        if(componentType=="UNDEFINED")
            Log::wrn(LOG_TAG, "Component type is not set.");
        return componentType;
    }
}
