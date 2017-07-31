//
// Created by Ethan Breit on 2017-07-25.
//

#pragma once

#include <gl/glew.h>

#include <graphics/types/Uniform.h>
#include <string>

namespace ge
{
    namespace GL
    {
        struct Uniform : ge::Uniform
        {
            std::string identifier;
            int id;

            void setData(float);

            void setData(glm::vec2);

            void setData(glm::vec3);

            void setData(glm::vec4);

            void setData(int);

            void setData(glm::tvec2<int>);

            void setData(glm::tvec3<int>);

            void setData(glm::tvec4<int>);

            void setData(double);

            void setData(glm::tvec2<double>);

            void setData(glm::tvec3<double>);

            void setData(glm::tvec4<double>);

        };
    }
}