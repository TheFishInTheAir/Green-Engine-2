#include <ge/graphics/UniformAutomator.h>
#include <engine/global_settings.pre>
#include <ge/engine/scene/Scene.h>

#include <ge/console/Log.h>
namespace ge
{
    namespace UniformAutomator
    {
        void initUniformSetup(ge::TriangleMesh* mesh)
        {
            //NOTE: probably not the best way to do this...


            //Textures
            if(mesh->containsUniform(DBL_STRINGIFY(ALBEDO_REF)))
                mesh->setUniform(DBL_STRINGIFY(ALBEDO_REF), Uniform::UniformContent(std::stoi(DBL_STRINGIFY(ALBEDO_LOC))));
            if(mesh->containsUniform(DBL_STRINGIFY(SPECULAR_REF)))
                mesh->setUniform(DBL_STRINGIFY(SPECULAR_REF), Uniform::UniformContent(std::stoi(DBL_STRINGIFY(SPECULAR_LOC))));
            if(mesh->containsUniform(DBL_STRINGIFY(NORMAL_REF)))
                mesh->setUniform(DBL_STRINGIFY(NORMAL_REF), Uniform::UniformContent(std::stoi(DBL_STRINGIFY(NORMAL_LOC))));
            if(mesh->containsUniform(DBL_STRINGIFY(ROUGHNESS_REF)))
                mesh->setUniform(DBL_STRINGIFY(ROUGHNESS_REF), Uniform::UniformContent(std::stoi(DBL_STRINGIFY(ROUGHNESS_LOC))));
            
        }

        void cycledUniformSetup(ge::TriangleMesh* mesh)
        {
            if (mesh->containsUniform(DBL_STRINGIFY(CAMERA_DIR)) && Camera::displayCamera != nullptr)
				mesh->setUniform(DBL_STRINGIFY(CAMERA_DIR), ge::Uniform::UniformContent(Camera::displayCamera->dir));
            if (mesh->containsUniform(DBL_STRINGIFY(CAMERA_POS)) && Camera::displayCamera != nullptr)
				mesh->setUniform(DBL_STRINGIFY(CAMERA_POS), ge::Uniform::UniformContent(Camera::displayCamera->pos));

            if (mesh->containsUniform(DBL_STRINGIFY(GE_TIME)))
            {
                static Runtime* r = RuntimeManager::getRuntime(RUNTIME_MAIN);
                //Log::dbg(std::to_string(r->getLastDelta()));
                mesh->setUniform(DBL_STRINGIFY(GE_TIME), ge::Uniform::UniformContent(r->getTime())); //arbitrary speed

                
            }

            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_NUM)))
            	mesh->setUniform(DBL_STRINGIFY(LIGHT_NUM), ge::Uniform::UniformContent((int)Scene::currentScene->lights.size()));

            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_IN))) //NOTE: there must be a faster way of doing this
            {
                int i = 0;
                for(auto light : Scene::currentScene->lights)
                { 
                    std::string prefix = std::string() + DBL_STRINGIFY(LIGHT_IN)+"["+ std::to_string(i++) +"].";
                    //Log::dbg("test");
                    mesh->setUniform(prefix+"colour", ge::Uniform::UniformContent(light->colour));
                    mesh->setUniform(prefix+"pos", ge::Uniform::UniformContent(light->pos));
                    mesh->setUniform(prefix+"dir", ge::Uniform::UniformContent(light->dir));
                    mesh->setUniform(prefix+"light_type", ge::Uniform::UniformContent(light->light_type));
                    mesh->setUniform(prefix+"angle", ge::Uniform::UniformContent(light->angle));

                }
            }


        }

        void cycledEntUniformSetup(ge::Entity* ent, ge::TriangleMesh* mesh)
        {

        }

    }
}