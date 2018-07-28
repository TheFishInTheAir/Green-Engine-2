#pragma once
namespace ge
{
    namespace DataType
    {
        typedef enum
        {

            STRING,

            BOOL,

            INT,

            FLOAT,
            DOUBLE,

            FVEC2,
            FVEC3,
            FVEC4,

            FMAT2,
            FMAT3,
            FMAT4,

            QUAT,

            SHADER,
            SHADER_GROUP,

            MESH_DATA,
            MATERIAL,

            TEXTURE,
            CUBEMAP,

            COLOUR3, //vec3 but with a nice UI
            COLOUR4  //vec4 but with a nice UI

        } type;
    }
}