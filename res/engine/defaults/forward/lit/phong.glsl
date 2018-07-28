#version 330

#pragma include <engine/global_settings.pre>
#pragma include <engine/shader/lighting/lights.glsl>

#ifdef VERTEX

layout(location = VERTEX_POSITION) in vec3 vpos;
layout(location = UV_POSITION)     in vec2 texcoord;
layout(location = NORMAL_POSITION) in vec3 normals;

uniform mat4 MVP_M;
uniform mat4 MVP_VP;

out vec3 tNormals;
out vec2 tTexcoord;
out vec4 tFragcoord;

void main()
{
    gl_Position = MVP_M_VP * vec4(vpos, 1);


    mat3 normalMatrix = transpose(inverse(mat3(MVP_M))); //TODO: calculate normal matrix on CPU

    tFragcoord = MVP_M * vec4(vpos, 1);
	tNormals = normalize(normalMatrix * normals);
    tTexcoord = texcoord;
}

#elif defined FRAGMENT

out vec4 colour;

in vec3 tNormals;
in vec2 tTexcoord;
in vec4 tFragcoord; //World space



uniform vec3 CAMERA_POS;

uniform sampler2D ALBEDO_REF;
uniform sampler2D SPECULAR_REF;
//TODO: add normal maps

uniform Light LIGHT_IN[MAX_LIGHTS];
uniform int LIGHT_NUM;

//Phong Lighting

vec3 computeDirectionalLight(Light l)
{
    vec4 lCol = vec4(normalize(l.colour), 1); //the normalize is just a test
    vec4 colour = vec4(0);

    vec3 normalLightDir = normalize(l.dir);

    //AMBIENT
    colour += texture(ALBEDO_REF, tTexcoord) * vec4(0.1);

    //Diffuse
    colour +=
        texture(ALBEDO_REF, tTexcoord) *
        max(dot(-normalLightDir, tNormals),0) *
        lCol;

    //Specular
    float spec = dot(reflect(normalLightDir, tNormals),
                     normalize(CAMERA_POS-vec3(tFragcoord)));
    colour +=
        pow(max(spec,0),32) * //32 is an arbitrary default, I might change it to 64 or something
        lCol *
        texture(SPECULAR_REF, tTexcoord);

    return vec3(colour);
}



vec3 computeLights()
{
    vec3 currentColour = vec3(0);

    for(int i = 0; i < LIGHT_NUM; i++)
    {
        Light currentLight = LIGHT_IN[i];

        switch(currentLight.light_type)
        {
        case LIGHT_DIRECTIONAL:
            currentColour += computeDirectionalLight(currentLight);
            break;
        }

    }
    return currentColour;
}

void main()
{
    //vec3 colour = vec3(0);

    //AMBIENT
    //colour += vec3(0.1);

    //Lighting
    colour = vec4(computeLights(), 1);
    //colour = vec4(1,1,1,1);
}


#endif
