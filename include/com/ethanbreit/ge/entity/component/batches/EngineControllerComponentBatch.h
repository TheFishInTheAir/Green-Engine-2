#pragma once
#include <ge/entity/component/ComponentBatch.h>
#include <ge/runtime/Runtime.h>
#include <engine/global_settings.pre>

namespace ge
{
    struct EngineControllerComponentBatch : public ComponentBatch
    {
        EngineControllerComponentBatch();
        
        std::vector<Component*> components;
        
        void cycle();
        
        ge_START_CYCLE_HANDLER(EngineControllerComponentBatch)
        ge_GENERATE_TRAMPOLINE(cycle, POST_RENDER)
        ge_END_CYCLE_HANDLER
        
        
        void softInsert(Component*);
        void softRemove(Component*);
        void softRemove(uint32_t);
        
        uint32_t hardInsert(Component*);
        void hardRemove(uint32_t);
        
    private:
        typedef enum
        {INSERT, REMOVE} _action;
        std::queue<std::pair<_action, Component*>> actionBuffer;
        
        std::queue<uint32_t> emptyIndicies;
        
        void performPendingActions();
    };
}
