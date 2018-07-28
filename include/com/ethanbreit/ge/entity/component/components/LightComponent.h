#pragma once
#include <ge/entity/component/Component.h>
#include <ge/graphics/meshes/TriangleMesh.h>
#include <ge/graphics/types/Light.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/ComponentConstructorRegistry.h>
#include <vector>
namespace ge
{
    struct LightComponent : public Component
    {
        LightComponent(Entity*);

        void defaultInit();
        void insertToDefaultBatch();
        
        void cycle();
        void destroy();
        std::string getTypeName();
        
        Light light;
    private:
        int lightIter = 0;

        TransformComponent* transformComponent = nullptr;
        static ComponentConstructorRegistry::StartupHook _hook;

    };
}
