#version 330

#define FRAGMENT

#define NO_UNIFORMS //TODO: finish uniform work.
#pragma include <engine/shader/lighting/lights.glsl>


out vec4 colour;


in vec2 tUV;
in vec3 tNorm;


uniform sampler2D tex;
uniform DirectionalLight testLight; //TODO: use proper lighting uniforms.


void main()
{

	colour = texture(tex, tUV) * calcDirectionalLightDIF(testLight, tNorm);
	//colour = vec4(tUV.x,tUV.y,0,1);
}
