//
// Created by Ethan Breit on 2017-07-25.
//

#include <OpenGL/types/GLUniform.h>
#include <OpenGL/Meshes/GLTriangleMesh.h>

void ge::GL::Uniform::setData(float f)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform1f(id, f);
}

void ge::GL::Uniform::setData(glm::vec2 vf)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform2f(id, vf.x, vf.y);
}

void ge::GL::Uniform::setData(glm::vec3 vf)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform3f(id, vf.x, vf.y, vf.z);
}

void ge::GL::Uniform::setData(glm::vec4 vf)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform4f(id, vf.x, vf.y, vf.z, vf.w);
}

void ge::GL::Uniform::setData(int32_t i)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform1i(id, i);
}

void ge::GL::Uniform::setData(glm::tvec2<int32_t> vi)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform2i(id, vi.x, vi.y);
}

void ge::GL::Uniform::setData(glm::tvec3<int32_t> vi)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform3i(id, vi.x, vi.y, vi.z);
}

void ge::GL::Uniform::setData(glm::tvec4<int32_t> vi)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform4i(id, vi.x, vi.y, vi.z, vi.w);
}

void ge::GL::Uniform::setData(double d)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform1d(id, d);
}

void ge::GL::Uniform::setData(glm::tvec2<double> vd)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform2d(id, vd.x, vd.y);
}

void ge::GL::Uniform::setData(glm::tvec3<double> vd)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform3d(id, vd.x, vd.y, vd.z);
}

void ge::GL::Uniform::setData(glm::tvec4<double> vd)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniform4d(id, vd.x, vd.y, vd.z, vd.w);
}

void ge::GL::Uniform::setData(glm::mat2 m)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniformMatrix2fv(id,1,GL_FALSE,&m[0][0]);
}

void ge::GL::Uniform::setData(glm::mat3 m)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniformMatrix3fv(id,1,GL_FALSE,&m[0][0]);

}

void ge::GL::Uniform::setData(glm::mat4 m)
{
	static_cast<GL::TriangleMesh*>(parentMesh)->startRender();
    glUniformMatrix4fv(id,1,GL_FALSE,&m[0][0]);

}
