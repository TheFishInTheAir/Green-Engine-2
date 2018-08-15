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

uniform DirLight LIGHT_DIR_IN[LIGHT_DIR_MAX];
uniform int LIGHT_DIR_NUM;

uniform PointLight LIGHT_POINT_IN[LIGHT_POINT_MAX];
uniform int LIGHT_POINT_NUM;

//Phong Lighting

vec3 computeDirectionalLight(DirLight l)
{
    vec4 lCol = vec4(normalize(l.colour), 1); //the normalize is just a test
    vec4 colour = vec4(0);

    vec3 normalLightDir = normalize(l.dir);

    //AMBIENT
    vec4 ambient = l.ambient * lCol;

    //Diffuse
    vec4 diff = 
        max(dot(-normalLightDir, tNormals),0) *
        lCol;


    //Specular
    float specAmnt = dot(reflect(normalLightDir, tNormals),
                     normalize(CAMERA_POS-vec3(tFragcoord)));
    vec4 spec =
        pow(max(specAmnt,0),32) * //32 is an arbitrary default, I might change it to 64 or something
        lCol *
        texture(SPECULAR_REF, tTexcoord);

    colour += texture(ALBEDO_REF, tTexcoord) * (ambient + diff + spec);


    return vec3(colour);
}

vec3 computePointLight(PointLight l)
{
    vec4 lCol = vec4(normalize(l.colour), 1); //the normalize is just a test
    vec4 colour = vec4(0);

    vec3 normalLightDir = normalize(tFragcoord.xyz - l.pos);

    //Attenuation
    float distance    = length(tFragcoord.xyz - l.pos );
    float attenuation = 1.0 / (l.constant + l.linear * distance +
                l.quadratic * (distance * distance));

    //AMBIENT
    //vec4 ambient = l.ambient * lCol * attenuation;

    //Diffuse
    vec4 diff = 
        max(dot(-normalLightDir, tNormals),0) *
        lCol * attenuation;


    //Specular
    float specAmnt = dot(reflect(normalLightDir, tNormals),
                     normalize(CAMERA_POS-vec3(tFragcoord)));
    vec4 spec =
        pow(max(specAmnt,0),32) * //32 is an arbitrary default, I might change it to 64 or something
        lCol *
        texture(SPECULAR_REF, tTexcoord) * attenuation;

    colour += texture(ALBEDO_REF, tTexcoord) * (diff + spec);


    return vec3(colour);
}



vec3 computeLights()
{
    vec3 currentColour = vec3(0);

    for(int i = 0; i < LIGHT_DIR_NUM; i++)
        currentColour += computeDirectionalLight(LIGHT_DIR_IN[i]);
    for(int i = 0; i < LIGHT_POINT_NUM; i++)
        currentColour += computePointLight(LIGHT_POINT_IN[i]);


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
