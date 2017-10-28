#version 330

#define FRAGMENT

#define NO_UNIFORMS //TODO: finish uniform work.
#pragma include <engine/shader/lighting/lights.glsl>


out vec4 colour;


in vec2 tUV;
in vec3 tNorm;
in vec3 tFragPos;

uniform sampler2D tex;

uniform vec3 CAMERA_POS;

uniform DirectionalLight testLight; //TODO: use proper lighting uniforms.


void main()
{
	vec4 intermediateColour = texture(tex, tUV);
	float spec = pow(calcDirectionalLightSPEC(testLight, tNorm, CAMERA_POS, tFragPos), 32);
	intermediateColour *= calcDirectionalLightDIF(testLight, tNorm);
	intermediateColour += vec4(testLight.colour, 0) * spec * 0.5f;
	colour = intermediateColour;
}
