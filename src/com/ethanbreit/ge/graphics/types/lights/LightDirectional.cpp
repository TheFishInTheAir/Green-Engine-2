#include <ge/graphics/types/lights/LightDirectional.h>
namespace ge
{

    void LightDirectional::pushUnifValues(TriangleMesh* mesh, std::string prefix)
    {
        mesh->setUniform(prefix+"colour", colour);
        mesh->setUniform(prefix+"ambient", ambient);
        mesh->setUniform(prefix+"dir", dir);
    }
}