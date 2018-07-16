#pragma once
#include <ge/entity/component/Component.h>

namespace ge
{
    struct TransformComponent;
    struct FreeCamControllerComponent : public Component
    {
        FreeCamControllerComponent(Entity* e) : Component(e){}

        void defaultInit();
        void insertToDefaultBatch();

        void cycle();
        void destroy();
        std::string getTypeName();
        
        float moveSpeed = 0.0035f;
        float lookSpeed = 0.0015f;
        
        float sprintModif = 2;
        
        
    private:
        bool hasTransformComponent = false;
        TransformComponent* tComp = nullptr;
        
        float horizontalAngle = 0;
        float verticalAngle = 0;
    };
}
