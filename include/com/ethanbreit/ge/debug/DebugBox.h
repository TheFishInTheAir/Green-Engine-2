#pragma once
#include <glm/glm.hpp>
namespace ge
{
    namespace Debug
    {
        namespace DebugBox
        {
            void draw(glm::vec3, glm::vec3, glm::vec3, glm::mat4);
            void draw(glm::vec3, glm::vec3, glm::vec3);
            
        }
    }
}