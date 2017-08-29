#version 330
out vec4 colour;
in vec2 tUV;

uniform sampler2D tex;


void main()
{
	colour = texture(tex, tUV);
}