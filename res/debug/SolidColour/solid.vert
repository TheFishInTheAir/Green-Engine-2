#version 330

#pragma include <engine/shader/global_settings.pre>

layout(location = VERTEX_POSITION) in vec3 vertexPositions;

uniform mat4 mvp;

out vec3 col;

void main()
{
  gl_Position = mvp * vec4(vertexPositions,1);
}
