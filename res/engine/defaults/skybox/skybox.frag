#version 330

#define FRAGMENT

#pragma include <ge/engine/global_settings.pre>

out vec4 outColour;

in vec3 texCoords;

uniform samplerCube CUBEMAP_0;

void main()
{

	outColour = texture(CUBEMAP_0, texCoords);
	//outColour = vec4(texCoords, 1);
}
