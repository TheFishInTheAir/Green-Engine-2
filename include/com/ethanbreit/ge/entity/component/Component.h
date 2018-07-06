#pragma once
#include <string>

namespace ge
{
    
    struct ComponentBatch;
    struct Entity;
    
	struct Component
	{
        Component(Entity*);
        
        virtual std::string getTypeName() = 0;
        
        virtual ComponentBatch* getBatch();
        virtual void setBatch(ComponentBatch*);
        
        virtual void defaultInit() = 0; //I haven't needed to use it yet, maybe remove it? TODO: UPDATE
        virtual void insertToDefaultBatch() = 0;
        
		virtual void cycle() = 0;
        virtual void destroy() = 0;
        
        uint32_t batchId = 0;
        
        Entity* getEntity();
        
    protected:
        ComponentBatch* currentBatch = nullptr;
        Entity* ent;
	};
}
