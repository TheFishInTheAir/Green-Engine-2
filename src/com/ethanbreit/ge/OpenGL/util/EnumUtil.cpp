//
// Created by Ethan Breit on 2017-07-26.
//
#include <ge/graphics/abs/OpenGL/util/EnumUtil.h>
#include <ge/graphics/enums/TextureFilterType.h>

namespace ge
{
    namespace GL
    {
        namespace EnumUtil
        {
            unsigned int getTrueShaderType(ShaderType::type type)
            {
                switch (type)
                {
                    case ShaderType::Shader_Vertex:return GL_VERTEX_SHADER;
                    case ShaderType::Shader_Geometry:return GL_GEOMETRY_SHADER;
                        //case ShaderType::Shader_Tessellation:return GLEW_ARB_tessellation_shader;
                    case ShaderType::Shader_Fragment:return GL_FRAGMENT_SHADER;
                    default:return 0;
                }
            }
            unsigned int getTrueBufferMemoryType(BufferMemoryType::type type)
            {
                switch (type)
                {

                    case BufferMemoryType::Static:return GL_STATIC_DRAW;
                    case BufferMemoryType::Dynamic:return GL_DYNAMIC_DRAW;
                    case BufferMemoryType::Stream:return GL_STREAM_DRAW;
                }
            }
            unsigned int getTrueDataType(BasicDataType::type type)
            {
                switch (type)
                {


                    case BasicDataType::Int:return GL_INT;
                    case BasicDataType::Int2:return GL_INT_VEC2;
                    case BasicDataType::Int3:return GL_INT_VEC3;
                    case BasicDataType::Int4:return GL_INT_VEC4;
                    case BasicDataType::UInt:return GL_UNSIGNED_INT;
                    case BasicDataType::UInt2:return GL_UNSIGNED_INT_VEC2;
                    case BasicDataType::UInt3:return GL_UNSIGNED_INT_VEC3;
                    case BasicDataType::UInt4:return GL_UNSIGNED_INT_VEC4;
                    case BasicDataType::Float:return GL_FLOAT;
                    case BasicDataType::Float2:return GL_FLOAT_VEC2;
                    case BasicDataType::Float3:return GL_FLOAT_VEC3;
                    case BasicDataType::Float4:return GL_FLOAT_VEC4;
                    case BasicDataType::Double:return GL_DOUBLE;
                    case BasicDataType::Double2:return GL_DOUBLE_VEC2;
                    case BasicDataType::Double3:return GL_DOUBLE_VEC3;
                    case BasicDataType::Double4:return GL_DOUBLE_VEC4;

                }
            }

        }
    }
}