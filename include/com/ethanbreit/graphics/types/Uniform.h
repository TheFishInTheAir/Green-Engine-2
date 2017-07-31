//
// Created by Ethan Breit on 2017-07-25.
//

#pragma once

#include <glm/glm.hpp>

namespace ge
{
    struct Uniform
    {
        virtual void setData(float)             = 0;
        virtual void setData(glm::vec2)         = 0;
        virtual void setData(glm::vec3)         = 0;
        virtual void setData(glm::vec4)         = 0;

        virtual void setData(int)               = 0;
        virtual void setData(glm::tvec2<int>)   = 0;
        virtual void setData(glm::tvec3<int>)   = 0;
        virtual void setData(glm::tvec4<int>)   = 0;

        virtual void setData(double)               = 0;
        virtual void setData(glm::tvec2<double>)   = 0;
        virtual void setData(glm::tvec3<double>)   = 0;
        virtual void setData(glm::tvec4<double>)   = 0;
    };
}