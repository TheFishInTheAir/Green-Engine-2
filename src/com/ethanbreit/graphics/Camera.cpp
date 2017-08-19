//
// Created by Ethan Breit on 2017-08-07.
//
#include <graphics/Camera.h>

namespace ge
{

    void Camera::update()
    {

        proj = glm::perspective(fov,aspectRatio,nearCull,farCull);

        view = glm::lookAt(pos, dir, up);

        vp =  proj *view;

    }
}
