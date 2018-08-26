#include <ge/engine/enum/DataTypes.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace ge
{
    namespace DataType
    {
        nlohmann::json serialize(type t, void* val)
        {
            nlohmann::json jv;
            switch(t)
            {
            case ge::DataType::FLOAT:
                jv = *(float *)val;
                break;
            case ge::DataType::INT:
                jv = *(int *)val;
                break;
            case ge::DataType::DOUBLE:
                jv = *(double *)val;
                break;
            case ge::DataType::BOOL:
                jv = *(bool *)val;
                break;
            case ge::DataType::STRING:
                jv = *(std::string *)val;
                break;
            case ge::DataType::MESH_DATA:
                jv = *(std::string *)val;
                break;
            case ge::DataType::MATERIAL:
                jv = *(std::string *)val;
                break;
            case ge::DataType::SHADER:
                jv = *(std::string *)val;
                break;
            case ge::DataType::SHADER_GROUP:
                jv = *(std::string *)val;
                break;
            case ge::DataType::TEXTURE:
                jv = *(std::string *)val;
                break;
            case ge::DataType::CUBEMAP:
                jv = *(std::string *)val;
                break;
            case ge::DataType::FVEC2:
                jv["x"] = (*(glm::vec2 *)val).x;
                jv["y"] = (*(glm::vec2 *)val).y;
                break;
            case ge::DataType::FVEC3:
                jv["x"] = (*(glm::vec3 *)val).x;
                jv["y"] = (*(glm::vec3 *)val).y;
                jv["z"] = (*(glm::vec3 *)val).z;
                break;
            case ge::DataType::COLOUR3:
                jv["x"] = (*(glm::vec3 *)val).x;
                jv["y"] = (*(glm::vec3 *)val).y;
                jv["z"] = (*(glm::vec3 *)val).z;
                break;
            case ge::DataType::FVEC4:
                jv["x"] = (*(glm::vec4 *)val).x;
                jv["y"] = (*(glm::vec4 *)val).y;
                jv["z"] = (*(glm::vec4 *)val).z;
                jv["w"] = (*(glm::vec4 *)val).w;
                break;
            case ge::DataType::COLOUR4:
                jv["x"] = (*(glm::vec4 *)val).x;
                jv["y"] = (*(glm::vec4 *)val).y;
                jv["z"] = (*(glm::vec4 *)val).z;
                jv["w"] = (*(glm::vec4 *)val).w;
                break;
            case ge::DataType::QUAT:
                jv["x"] = (*(glm::quat *)val).x;
                jv["y"] = (*(glm::quat *)val).y;
                jv["z"] = (*(glm::quat *)val).z;
                jv["w"] = (*(glm::quat *)val).w;
                break;

            case ge::DataType::FMAT2:
                jv["m00"] = (*(glm::mat2 *)val)[0][0];
                jv["m01"] = (*(glm::mat2 *)val)[0][1];
                jv["m10"] = (*(glm::mat2 *)val)[1][0];
                jv["m11"] = (*(glm::mat2 *)val)[1][1];
                break;
            case ge::DataType::FMAT3:
                jv["m00"] = (*(glm::mat3 *)val)[0][0];
                jv["m01"] = (*(glm::mat3 *)val)[0][1];
                jv["m02"] = (*(glm::mat3 *)val)[0][2];

                jv["m10"] = (*(glm::mat3 *)val)[1][0];
                jv["m11"] = (*(glm::mat3 *)val)[1][1];
                jv["m12"] = (*(glm::mat3 *)val)[1][2];

                jv["m20"] = (*(glm::mat3 *)val)[2][0];
                jv["m21"] = (*(glm::mat3 *)val)[2][1];
                jv["m22"] = (*(glm::mat3 *)val)[2][2];
                break;

            case ge::DataType::FMAT4:
                jv["m00"] = (*(glm::mat4 *)val)[0][0];
                jv["m01"] = (*(glm::mat4 *)val)[0][1];
                jv["m02"] = (*(glm::mat4 *)val)[0][2];
                jv["m03"] = (*(glm::mat4 *)val)[0][3];

                jv["m10"] = (*(glm::mat4 *)val)[1][0];
                jv["m11"] = (*(glm::mat4 *)val)[1][1];
                jv["m12"] = (*(glm::mat4 *)val)[1][2];
                jv["m13"] = (*(glm::mat4 *)val)[1][3];

                jv["m20"] = (*(glm::mat4 *)val)[2][0];
                jv["m21"] = (*(glm::mat4 *)val)[2][1];
                jv["m22"] = (*(glm::mat4 *)val)[2][2];
                jv["m23"] = (*(glm::mat4 *)val)[2][3];

                jv["m30"] = (*(glm::mat4 *)val)[3][0];
                jv["m31"] = (*(glm::mat4 *)val)[3][1];
                jv["m32"] = (*(glm::mat4 *)val)[3][2];
                jv["m33"] = (*(glm::mat4 *)val)[3][3];
                break;
            }
            return jv;
        }

        void copy(type t, void* src, void* dest)
        {
            switch (t)
            {
            case ge::DataType::FLOAT:
                (*(float *)dest) = (*(float *)src);
                break;
            case ge::DataType::INT:
                (*(int *)dest) = (*(int *)src);
                break;
            case ge::DataType::DOUBLE:
                (*(double *)dest) = (*(double *)src);
                break;
            case ge::DataType::BOOL:
                (*(bool *)dest) = (*(bool *)src);
                break;
            case ge::DataType::STRING:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::MESH_DATA:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::MATERIAL:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::SHADER:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::SHADER_GROUP:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::TEXTURE:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::CUBEMAP:
                (*(std::string *)dest) = (*(std::string *)src);
                break;
            case ge::DataType::FVEC2:
                (*(glm::vec2 *)dest).x = (*(glm::vec2 *)src).x;
                (*(glm::vec2 *)dest).y = (*(glm::vec2 *)src).y;
                break;
            case ge::DataType::FVEC3:
                (*(glm::vec3 *)dest).x = (*(glm::vec3 *)src).x;
                (*(glm::vec3 *)dest).y = (*(glm::vec3 *)src).y;
                (*(glm::vec3 *)dest).z = (*(glm::vec3 *)src).z;
                break;
            case ge::DataType::COLOUR3:
                (*(glm::vec3 *)dest).x = (*(glm::vec3 *)src).x;
                (*(glm::vec3 *)dest).y = (*(glm::vec3 *)src).y;
                (*(glm::vec3 *)dest).z = (*(glm::vec3 *)src).z;
                break;
            case ge::DataType::FVEC4:
                (*(glm::vec4 *)dest).x = (*(glm::vec4 *)src).x;
                (*(glm::vec4 *)dest).y = (*(glm::vec4 *)src).y;
                (*(glm::vec4 *)dest).z = (*(glm::vec4 *)src).z;
                (*(glm::vec4 *)dest).w = (*(glm::vec4 *)src).w;
                break;
            case ge::DataType::COLOUR4:
                (*(glm::vec4 *)dest).x = (*(glm::vec4 *)src).x;
                (*(glm::vec4 *)dest).y = (*(glm::vec4 *)src).y;
                (*(glm::vec4 *)dest).z = (*(glm::vec4 *)src).z;
                (*(glm::vec4 *)dest).w = (*(glm::vec4 *)src).w;
                break;
            case ge::DataType::QUAT:
                (*(glm::quat *)dest).x = (*(glm::quat *)src).x;
                (*(glm::quat *)dest).y = (*(glm::quat *)src).y;
                (*(glm::quat *)dest).z = (*(glm::quat *)src).z;
                (*(glm::quat *)dest).w = (*(glm::quat *)src).w;
                break;

            case ge::DataType::FMAT2:
                (*(glm::mat2 *)dest)[0][0] = (*(glm::mat2 *)src)[0][0];
                (*(glm::mat2 *)dest)[0][1] = (*(glm::mat2 *)src)[0][1];
                (*(glm::mat2 *)dest)[1][0] = (*(glm::mat2 *)src)[1][0];
                (*(glm::mat2 *)dest)[1][1] = (*(glm::mat2 *)src)[1][1];
                break;
            case ge::DataType::FMAT3:
                (*(glm::mat3 *)dest)[0][0] = (*(glm::mat3 *)src)[0][0];
                (*(glm::mat3 *)dest)[0][1] = (*(glm::mat3 *)src)[0][1];
                (*(glm::mat3 *)dest)[0][2] = (*(glm::mat3 *)src)[0][2];

                (*(glm::mat3 *)dest)[1][0] = (*(glm::mat3 *)src)[1][0];
                (*(glm::mat3 *)dest)[1][1] = (*(glm::mat3 *)src)[1][1];
                (*(glm::mat3 *)dest)[1][2] = (*(glm::mat3 *)src)[1][2];

                (*(glm::mat3 *)dest)[2][0] = (*(glm::mat3 *)src)[2][0];
                (*(glm::mat3 *)dest)[2][1] = (*(glm::mat3 *)src)[2][1];
                (*(glm::mat3 *)dest)[2][2] = (*(glm::mat3 *)src)[2][2];
                break;

            case ge::DataType::FMAT4:
                (*(glm::mat4 *)dest)[0][0] = (*(glm::mat4 *)src)[0][0];
                (*(glm::mat4 *)dest)[0][1] = (*(glm::mat4 *)src)[0][1];
                (*(glm::mat4 *)dest)[0][2] = (*(glm::mat4 *)src)[0][2];
                (*(glm::mat4 *)dest)[0][3] = (*(glm::mat4 *)src)[0][3];

                (*(glm::mat4 *)dest)[1][0] = (*(glm::mat4 *)src)[1][0];
                (*(glm::mat4 *)dest)[1][1] = (*(glm::mat4 *)src)[1][1];
                (*(glm::mat4 *)dest)[1][2] = (*(glm::mat4 *)src)[1][2];
                (*(glm::mat4 *)dest)[1][3] = (*(glm::mat4 *)src)[1][3];

                (*(glm::mat4 *)dest)[2][0] = (*(glm::mat4 *)src)[2][0];
                (*(glm::mat4 *)dest)[2][1] = (*(glm::mat4 *)src)[2][1];
                (*(glm::mat4 *)dest)[2][2] = (*(glm::mat4 *)src)[2][2];
                (*(glm::mat4 *)dest)[2][3] = (*(glm::mat4 *)src)[2][3];

                (*(glm::mat4 *)dest)[3][0] = (*(glm::mat4 *)src)[3][0];
                (*(glm::mat4 *)dest)[3][1] = (*(glm::mat4 *)src)[3][1];
                (*(glm::mat4 *)dest)[3][2] = (*(glm::mat4 *)src)[3][2];
                (*(glm::mat4 *)dest)[3][3] = (*(glm::mat4 *)src)[3][3];
                break;
            }
        }


        void deserialize(nlohmann::json jVar, type t, void *oVar)
        {
            switch (t)
            {
            case ge::DataType::FLOAT:
                (*(float *)oVar) = jVar;
                break;
            case ge::DataType::INT:
                (*(int *)oVar) = jVar;
                break;
            case ge::DataType::DOUBLE:
                (*(double *)oVar) = jVar;
                break;
            case ge::DataType::BOOL:
                (*(bool *)oVar) = jVar;
                break;
            case ge::DataType::STRING:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::MESH_DATA:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::MATERIAL:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::SHADER:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::SHADER_GROUP:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::TEXTURE:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::CUBEMAP:
                (*(std::string *)oVar) = jVar;
                break;
            case ge::DataType::FVEC2:
                (*(glm::vec2 *)oVar).x = jVar["x"];
                (*(glm::vec2 *)oVar).y = jVar["y"];
                break;
            case ge::DataType::FVEC3:
                (*(glm::vec3 *)oVar).x = jVar["x"];
                (*(glm::vec3 *)oVar).y = jVar["y"];
                (*(glm::vec3 *)oVar).z = jVar["z"];
                break;
            case ge::DataType::COLOUR3:
                (*(glm::vec3 *)oVar).x = jVar["x"];
                (*(glm::vec3 *)oVar).y = jVar["y"];
                (*(glm::vec3 *)oVar).z = jVar["z"];
                break;
            case ge::DataType::FVEC4:
                (*(glm::vec4 *)oVar).x = jVar["x"];
                (*(glm::vec4 *)oVar).y = jVar["y"];
                (*(glm::vec4 *)oVar).z = jVar["z"];
                (*(glm::vec4 *)oVar).w = jVar["w"];
                break;
            case ge::DataType::COLOUR4:
                (*(glm::vec4 *)oVar).x = jVar["x"];
                (*(glm::vec4 *)oVar).y = jVar["y"];
                (*(glm::vec4 *)oVar).z = jVar["z"];
                (*(glm::vec4 *)oVar).w = jVar["w"];
                break;
            case ge::DataType::QUAT:
                (*(glm::quat *)oVar).x = jVar["x"];
                (*(glm::quat *)oVar).y = jVar["y"];
                (*(glm::quat *)oVar).z = jVar["z"];
                (*(glm::quat *)oVar).w = jVar["w"];
                break;

            case ge::DataType::FMAT2:
                (*(glm::mat2 *)oVar)[0][0] = jVar["m00"];
                (*(glm::mat2 *)oVar)[0][1] = jVar["m01"];
                (*(glm::mat2 *)oVar)[1][0] = jVar["m10"];
                (*(glm::mat2 *)oVar)[1][1] = jVar["m11"];
                break;
            case ge::DataType::FMAT3:
                (*(glm::mat3 *)oVar)[0][0] = jVar["m00"];
                (*(glm::mat3 *)oVar)[0][1] = jVar["m01"];
                (*(glm::mat3 *)oVar)[0][2] = jVar["m02"];

                (*(glm::mat3 *)oVar)[1][0] = jVar["m10"];
                (*(glm::mat3 *)oVar)[1][1] = jVar["m11"];
                (*(glm::mat3 *)oVar)[1][2] = jVar["m12"];

                (*(glm::mat3 *)oVar)[2][0] = jVar["m20"];
                (*(glm::mat3 *)oVar)[2][1] = jVar["m21"];
                (*(glm::mat3 *)oVar)[2][2] = jVar["m22"];
                break;

            case ge::DataType::FMAT4:
                (*(glm::mat4 *)oVar)[0][0] = jVar["m00"];
                (*(glm::mat4 *)oVar)[0][1] = jVar["m01"];
                (*(glm::mat4 *)oVar)[0][2] = jVar["m02"];
                (*(glm::mat4 *)oVar)[0][3] = jVar["m03"];

                (*(glm::mat4 *)oVar)[1][0] = jVar["m10"];
                (*(glm::mat4 *)oVar)[1][1] = jVar["m11"];
                (*(glm::mat4 *)oVar)[1][2] = jVar["m12"];
                (*(glm::mat4 *)oVar)[1][3] = jVar["m13"];

                (*(glm::mat4 *)oVar)[2][0] = jVar["m20"];
                (*(glm::mat4 *)oVar)[2][1] = jVar["m21"];
                (*(glm::mat4 *)oVar)[2][2] = jVar["m22"];
                (*(glm::mat4 *)oVar)[2][3] = jVar["m23"];

                (*(glm::mat4 *)oVar)[3][0] = jVar["m30"];
                (*(glm::mat4 *)oVar)[3][1] = jVar["m31"];
                (*(glm::mat4 *)oVar)[3][2] = jVar["m32"];
                (*(glm::mat4 *)oVar)[3][3] = jVar["m33"];
                break;
            }
        }
        
    }
}