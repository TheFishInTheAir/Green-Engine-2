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
            unsigned int getTrueDataType(DataType::type type)
            {
                switch (type)
                {


                    case DataType::Int:return GL_INT;
                    case DataType::Int2:return GL_INT_VEC2;
                    case DataType::Int3:return GL_INT_VEC3;
                    case DataType::Int4:return GL_INT_VEC4;
                    case DataType::UInt:return GL_UNSIGNED_INT;
                    case DataType::UInt2:return GL_UNSIGNED_INT_VEC2;
                    case DataType::UInt3:return GL_UNSIGNED_INT_VEC3;
                    case DataType::UInt4:return GL_UNSIGNED_INT_VEC4;
                    case DataType::Float:return GL_FLOAT;
                    case DataType::Float2:return GL_FLOAT_VEC2;
                    case DataType::Float3:return GL_FLOAT_VEC3;
                    case DataType::Float4:return GL_FLOAT_VEC4;
                    case DataType::Double:return GL_DOUBLE;
                    case DataType::Double2:return GL_DOUBLE_VEC2;
                    case DataType::Double3:return GL_DOUBLE_VEC3;
                    case DataType::Double4:return GL_DOUBLE_VEC4;

                }
            }

        }
    }
}