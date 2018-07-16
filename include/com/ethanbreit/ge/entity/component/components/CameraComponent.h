#pragma once
#include <ge/entity/component/Component.h>
#include <ge/graphics/Camera.h>
namespace ge
{
    struct TransformComponent;
    struct CameraComponent : public Component
    {
        CameraComponent(Entity* e) : Component(e){};

        void defaultInit();
        void insertToDefaultBatch();

        void cycle();
        void destroy();
        std::string getTypeName();

        Camera camera;
        void makeDisplay();
    private:
        bool hasTransformComponent = false;
        TransformComponent* transformComponent;
    };

}
