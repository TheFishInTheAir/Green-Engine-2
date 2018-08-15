#include <ge/entity/component/ComponentConstructorRegistry.h>
#include <unordered_map>
#include <ge/console/Log.h>
namespace ge
{
    namespace ComponentConstructorRegistry
    {
        std::unordered_map<std::string, Component*(*)(Entity*)>* _constructor_map;

        void init()
        {
            static bool initialised = false;
            if(initialised)
                return;
            initialised = true;
            Log::dbg("ComponentConstructorRegistry", "initilaising...");
            _constructor_map = new std::unordered_map<std::string, Component*(*)(Entity*)>();
        }

        StartupHook::StartupHook(std::string name, Component*(*c)(Entity*))
        {
            init();
            (*_constructor_map)[name] = c;
            ge::Log::scc("ComponentConstructorRegistry","Registered: "+name);
        }

        bool componentExists(std::string name)
        {
            init();
            return (*_constructor_map).count(name);
        }

        Component* newComponent(std::string name, Entity* ent)
        {
            init();
            if((*_constructor_map).count(name))
                return (*_constructor_map)[name](ent);
            else
                ge::Log::critErr("ComponentConstructorRegistry", "No component '"+name+"' exists in the Component Constructor Registry.");
            return nullptr;
        }   

        std::list<std::string> getAvailable()
        {
            init();
            std::list<std::string> available;
            for(auto cc : (*_constructor_map))
                available.push_front(cc.first);
            return available;
        }
    }
}