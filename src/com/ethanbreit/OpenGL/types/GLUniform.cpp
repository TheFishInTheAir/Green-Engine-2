//
// Created by Ethan Breit on 2017-07-25.
//

#include <OpenGL/types/GLUniform.h>

void ge::GL::Uniform::setData(float f)
{
    glUniform1f(id, f);
}

void ge::GL::Uniform::setData(glm::vec2 vf)
{
    glUniform2f(id, vf.x, vf.y);
}

void ge::GL::Uniform::setData(glm::vec3 vf)
{
    glUniform3f(id, vf.x, vf.y, vf.z);
}

void ge::GL::Uniform::setData(glm::vec4 vf)
{
    glUniform4f(id, vf.x, vf.y, vf.z, vf.w);
}

void ge::GL::Uniform::setData(int i)
{
    glUniform1i(id, i);
}

void ge::GL::Uniform::setData(glm::tvec2<int> vi)
{
    glUniform2i(id, vi.x, vi.y);
}

void ge::GL::Uniform::setData(glm::tvec3<int> vi)
{
    glUniform3i(id, vi.x, vi.y, vi.z);
}

void ge::GL::Uniform::setData(glm::tvec4<int> vi)
{
    glUniform4i(id, vi.x, vi.y, vi.z, vi.w);
}

void ge::GL::Uniform::setData(double d)
{
    glUniform1d(id, d);
}

void ge::GL::Uniform::setData(glm::tvec2<double> vd)
{
    glUniform2d(id, vd.x, vd.y);
}

void ge::GL::Uniform::setData(glm::tvec3<double> vd)
{
    glUniform3d(id, vd.x, vd.y, vd.z);
}

void ge::GL::Uniform::setData(glm::tvec4<double> vd)
{
    glUniform4d(id, vd.x, vd.y, vd.z, vd.w);
}
