//
// Created by Ethan Breit on 2017-08-12.
//

#include <ge/runtime/RuntimeGroup.h>

namespace ge
{



    void ge::RuntimeGroup::cycle()
    {
        for(auto item : items)
            item.handler->func(item.ref,runtimeId);
    }

    void ge::RuntimeGroup::remove(void* ref)
    {
        for(std::list<RuntimeManager::RuntimeItem>::iterator i = items.begin(); i!=items.end(); ++i)
			if (i->ref == ref) 
			{
				items.erase(i); 
				return;
			}
    }

    void RuntimeGroup::insert(void *ref, uint64_t uuid)
    {
        items.push_back({RuntimeManager::getHandler(uuid), ref});
    }
}