#include <ge/graphics/UniformAutomator.h>
#include <engine/global_settings.pre>
#include <ge/engine/scene/Scene.h>
#include <ge/graphics/Camera.h>
#include <ge/runtime/RuntimeManager.h>
#include <ge/runtime/Runtime.h>
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
            if(mesh->containsUniform(DBL_STRINGIFY(SKYBOX)))
            {
                mesh->registerCubeMap(Scene::currentScene->cubemaps[Scene::currentScene->skyboxCubemapName].get(), std::stoi(DBL_STRINGIFY(SKYBOX_LOC)));
                mesh->setUniform(DBL_STRINGIFY(SKYBOX), Uniform::UniformContent(std::stoi(DBL_STRINGIFY(SKYBOX_LOC))));
            }
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

            //Directional Lights
            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_DIR_NUM)))
            	mesh->setUniform(DBL_STRINGIFY(LIGHT_DIR_NUM), ge::Uniform::UniformContent((int)Scene::currentScene->directionalLights.size()));

            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_DIR_IN))) //NOTE: there must be a faster way of doing this
            {
                int i = 0;
                for(auto light : Scene::currentScene->directionalLights)
                {
                    std::string prefix = std::string() + DBL_STRINGIFY(LIGHT_DIR_IN)+"["+ std::to_string(i++) +"].";
                    
                    light->pushUnifValues(mesh, prefix);
                }
            }

            //Point Lights
            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_POINT_NUM)))
            	mesh->setUniform(DBL_STRINGIFY(LIGHT_POINT_NUM), ge::Uniform::UniformContent((int)Scene::currentScene->pointLights.size()));

            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_POINT_IN))) //NOTE: there must be a faster way of doing this
            {
                int i = 0;
                for(auto light : Scene::currentScene->pointLights)
                {
                    std::string prefix = std::string() + DBL_STRINGIFY(LIGHT_POINT_IN)+"["+ std::to_string(i++) +"].";
                    
                    light->pushUnifValues(mesh, prefix);
                }
            }

            //Spot Lights
            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_SPOT_NUM)))
            	mesh->setUniform(DBL_STRINGIFY(LIGHT_SPOT_NUM), ge::Uniform::UniformContent((int)Scene::currentScene->spotLights.size()));

            if(mesh->containsUniform(DBL_STRINGIFY(LIGHT_SPOT_IN))) //NOTE: there must be a faster way of doing this
            {
                int i = 0;
                for(auto light : Scene::currentScene->spotLights)
                {
                    std::string prefix = std::string() + DBL_STRINGIFY(LIGHT_SPOT_IN)+"["+ std::to_string(i++) +"].";
                    
                    light->pushUnifValues(mesh, prefix);
                }
            }

            if(mesh->containsUniform(DBL_STRINGIFY(A_SHADOW_NUM)))
            	mesh->setUniform(DBL_STRINGIFY(A_SHADOW_NUM), ge::Uniform::UniformContent((int)Scene::currentScene->shadows.size()));

            if(mesh->containsUniform(DBL_STRINGIFY(A_SHADOW_NUM))) //NOTE: there must be a faster way of doing this
            {
                int i = 0;
                for(auto shadow : Scene::currentScene->shadows)
                {
                    std::string prefix_space = std::string() + DBL_STRINGIFY(A_SHADOW_SPACE_IN)+"["+ std::to_string(i) +"]";
                    std::string prefix_map = std::string() +   DBL_STRINGIFY(A_SHADOW_MAP_IN)  +"["+ std::to_string(i) +"]";    
                    mesh->registerTexture(shadow->shadowMap, A_SHADOW_LOC_START+i); //NOTE: REMOVE THIS, ONLY A TEST
                    mesh->setUniform(prefix_map, ge::Uniform::UniformContent(A_SHADOW_LOC_START+i));
                    mesh->setUniform(prefix_space, ge::Uniform::UniformContent(shadow->lightSpace));
                    
                    i++;
                }
            }

        }

        void cycledEntUniformSetup(ge::Entity* ent, ge::TriangleMesh* mesh)
        {

        }

    }
}