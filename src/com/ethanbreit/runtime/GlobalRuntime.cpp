//
// Created by Ethan Breit on 2017-08-13.
//
#include <runtime/GlobalRuntime.h>
#include <vector>
#include <console/ConsoleIO.h>
namespace ge
{
    namespace GlobalRuntime
    {

        static std::vector<RuntimeHandler*> globalHandlers;

        RuntimeHandler* getHandler(uint64_t uuid)
        {
            if(globalHandlers.size()!=0)
            {
                for (auto i : globalHandlers)
                {
                    if(i->uuid == uuid)
                        return i;
                }
            }
            return nullptr;
        }

        uint64_t genCycleHandlerUUID()
        {
            static uint64_t currentUUID = 1;
            currentUUID++;
            return currentUUID;
        }

        void registerHandler(uint64_t uuid, void (*func)(void *, uint32_t))
        {

            if(globalHandlers.size()!=0) /// Check if reference has already been registered.
            {
                for(auto i : globalHandlers)
                {
					if (i->uuid == uuid) /// If handler has already been registered, return.
					{
						ConsoleIO::print("Attempt to register already registered runtime reference.\n", MessageType::Warning);
						return;
					}
                }
            }
            RuntimeHandler* rh = new RuntimeHandler();
            rh->func = func;
            rh->uuid = uuid;
            globalHandlers.push_back(rh); /// Add handler to global list.
        }

    }
}