#include <ge/entity/component/ComponentConstructorRegistry.h>
#include <unordered_map>
namespace ge
{
    namespace ComponentConstructorRegistry
    {
        std::unordered_map<std::string, Component*(*)(Entity*)> _constructor_map;

        StartupHook::StartupHook(std::string name, Component*(*c)(Entity*))
        {
            _constructor_map[name] = c;
        }

        bool componentExists(std::string name)
        {
            return _constructor_map.count(name);
        }

        Component* newComponent(std::string name, Entity* ent)
        {
            return _constructor_map[name](ent);
        }

        std::list<std::string> getAvailable()
        {
            std::list<std::string> available;
            for(auto cc : _constructor_map)
                available.push_front(cc.first);
            return available;
        }
    }
}