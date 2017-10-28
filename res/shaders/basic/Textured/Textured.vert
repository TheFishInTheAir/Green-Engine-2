#version 330

#define VERTEX

#pragma include <engine/shader/global_settings.pre>

layout(location = VERTEX_POSITION) in vec3 vertexPositions;
layout(location = NORMAL_POSITION) in vec3 normals;
layout(location = UV_POSITION) in vec2 UV;

out vec2 tUV;
out vec3 tNorm;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(vertexPositions, 1);
	tUV = UV;
	tNorm = normals;
}
