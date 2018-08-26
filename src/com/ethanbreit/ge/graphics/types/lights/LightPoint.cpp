#include <ge/graphics/types/lights/LightPoint.h>

namespace ge
{
    void LightPoint::pushUnifValues(TriangleMesh* mesh, std::string prefix)
    {
        mesh->setUniform(prefix+"colour", colour);
        mesh->setUniform(prefix+"pos", pos);

        mesh->setUniform(prefix+"ambient", ambient);

        mesh->setUniform(prefix+"constant",  constant);
        mesh->setUniform(prefix+"linear",    linear);
        mesh->setUniform(prefix+"quadratic", quadratic);
        
    }
}