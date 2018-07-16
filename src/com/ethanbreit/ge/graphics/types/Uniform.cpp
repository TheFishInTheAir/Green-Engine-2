#include <ge/graphics/types/Uniform.h>
namespace ge
{
    Uniform::Uniform(Uniform::UniformType type)
    {
        this->type = type;
    }
    
    Uniform::UniformType Uniform::getUniformType()
    {
        return type;
    }
}
