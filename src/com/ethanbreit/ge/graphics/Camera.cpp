//
// Created by Ethan Breit on 2017-08-07.
//
#include <ge/graphics/Camera.h>

namespace ge
{
    Camera* Camera::displayCamera = nullptr;

    void Camera::update()
    {

        proj = glm::perspective(fov,aspectRatio,nearCull,farCull);

        view = glm::lookAt(pos, dir, up);

        vp =  proj *view;

    }
}
