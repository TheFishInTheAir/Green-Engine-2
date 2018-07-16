#pragma once
#include <ge/entity/component/Component.h>

namespace ge
{
    struct TestComponent : public ge::Component
    {
        TestComponent(Entity* e) : Component(e){};
        
        void defaultInit();
        void insertToDefaultBatch();
        
        void cycle();
        void destroy();
        std::string getTypeName();
        
    private:
        int i = 0;
        
    };
}
