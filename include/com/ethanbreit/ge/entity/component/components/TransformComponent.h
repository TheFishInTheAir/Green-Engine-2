#pragma once
#include <ge/entity/component/Component.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/gtx/quaternion.hpp>

namespace ge
{
    struct TransformComponent : public Component
    {
        //TODO: add camera stuff
        //TODO: make Transform struct and deprecate modelmat
        void defaultInit();
        void insertToDefaultBatch();

        void destroy();
        void cycle();
        std::string getTypeName();
        
        void setPosition(glm::vec3);
        void setScale(glm::vec3);
        void setRotation(glm::quat);
        
        glm::vec3 getPosition();
        glm::vec3 getScale();
        glm::quat getRotation();
        
        
        
        glm::mat4 getModel();
        
    private:
        glm::vec3 pos, scale;
        glm::quat rot;
        
        glm::mat4 model;
        
        bool shouldUpdate = false;
    };
}
