//
// Created by Ethan Breit on 2017-08-10.
//
#include <ge/util/RotationUtil.h>
namespace ge
{
    namespace RotationUtil
    {
        glm::vec3 getDirection(float verticalAngle, float horizontalAngle)
        {
            return glm::vec3(
                    cos(verticalAngle) * sin(horizontalAngle),
                    sin(verticalAngle),
                    cos(verticalAngle) * cos(horizontalAngle)
            );
        }
        glm::vec3 getRight(float horizontalAngle)
        {
            return glm::vec3(
                    sin(horizontalAngle - 3.14f / 2.0f),
                    0,
                    cos(horizontalAngle - 3.14f / 2.0f)
            );
        }
    }
}
