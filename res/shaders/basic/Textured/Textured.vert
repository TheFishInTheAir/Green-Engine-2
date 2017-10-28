#version 330

#define VERTEX

#pragma include <engine/shader/global_settings.pre>

layout(location = VERTEX_POSITION) in vec3 vertexPositions;
layout(location = NORMAL_POSITION) in vec3 normals;
layout(location = UV_POSITION) in vec2 UV;

out vec2 tUV;
out vec3 tNorm;
out vec3 tFragPos;
uniform mat4 model;
uniform mat4 vp;

void main()
{
	gl_Position = vp * model * vec4(vertexPositions, 1);
	tFragPos = vec3(model * vec4(vertexPositions, 1.0));
	tUV = UV;
	tNorm = normals;
}
