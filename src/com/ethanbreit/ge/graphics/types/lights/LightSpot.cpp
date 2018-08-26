#include <ge/graphics/types/lights/LightSpot.h>
#include <math.h>
namespace ge
{
    void LightSpot::pushUnifValues(TriangleMesh* mesh, std::string prefix)
    {
        mesh->setUniform(prefix+"colour", colour);
        mesh->setUniform(prefix+"pos", pos);
        mesh->setUniform(prefix+"dir", dir);

        mesh->setUniform(prefix+"ambient", ambient);

        mesh->setUniform(prefix+"angle",  (float)std::cos(glm::radians(angle)));
        mesh->setUniform(prefix+"outerAngle",  (float)std::cos(glm::radians(outerAngle+angle)));
        mesh->setUniform(prefix+"shadowLoc", shadowLoc);
        
    }
}