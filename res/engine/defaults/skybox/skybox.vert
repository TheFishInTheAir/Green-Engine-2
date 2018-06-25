#version 330


#define VERTEX

#pragma include <ge/engine/global_settings.pre>

layout(location = VERTEX_POSITION) in vec3 vertexPositions;

out vec3 texCoords;

uniform mat4 MVP_VP;

void main()
{
	vec4 pos = MVP_VP * vec4(vertexPositions, 1);

	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);

	texCoords = vertexPositions;

}
