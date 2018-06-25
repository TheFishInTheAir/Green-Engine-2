//
// Created by Ethan Breit on 2017-08-13.
//
#include <ge/runtime/RuntimeManager.h>
#include <vector>
#include <ge/console/ConsoleIO.h>
#include <unordered_map>
#include <ge/runtime/Runtime.h>
namespace ge
{
    namespace RuntimeManager
    {
		std::unordered_map<std::string, Runtime*> runtimes;

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

	    void registerRuntime(Runtime* runtime)
	    {
			runtimes.insert({ runtime->getName(), runtime });
	    }

	    Runtime* getRuntime(std::string s)
	    {
			return runtimes.find(s)->second;
	    }

	    void removeRuntime(std::string s)
	    {
			runtimes.erase(s);
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
