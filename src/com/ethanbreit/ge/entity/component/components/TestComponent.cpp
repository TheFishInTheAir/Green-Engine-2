#include <ge/entity/component/components/TestComponent.h>
#include <ge/console/Log.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <string>

namespace ge
{
    const std::string LOG_TAG = "TestCopmonent";
    
    void TestComponent::defaultInit()
    {
        //nothing
    }
    
    void TestComponent::insertToDefaultBatch()
    {
        if(!ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            ComponentBatch* cmp = new DefaultComponentBatch();
            cmp->setComponentType(getTypeName());

            ComponentManager::registerComponentBatch(cmp);
        }
        
        ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
    }
    
    void TestComponent::cycle()
    {
        Log::dbg(LOG_TAG, "Update");
    }
    
    void TestComponent::destroy()
    {
        Log::dbg(LOG_TAG, "Destroy");
    }
    
    std::string TestComponent::getTypeName()
    {
        return "TestComponent";
    }
}
