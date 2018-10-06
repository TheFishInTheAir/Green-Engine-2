#include <ge/engine/scene/SceneSerializer.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/EntitySerial.h>
#include <ge/entity/component/Component.h>
#include <glm/ext.hpp>
#include <ge/console/Log.h>
#include <ge/entity/EntityTag.h>

using json = nlohmann::json;
namespace ge
{
    namespace SceneSerializer
    {

        const int JSCENE_V_MAJOR = 1;
        const int JSCENE_V_MINOR = 2;


        void serializeSceneJson_Materials(json &jScene, Scene* s)
        {
            {//Textures
                std::list<std::string> textures;
                for(auto tm : s->textures)
                {
                    if(tm.second->url=="" || tm.second->url=="RUNTIME")
                        continue;
                        
                    textures.push_front(tm.first);
                }
                jScene["preloaded_resources"]["textures"] = textures;
            }

            {//Meshes
                std::list<std::string> meshes;
                for(auto mm : s->meshes)
                    meshes.push_front(mm.first);
                jScene["preloaded_resources"]["meshes"] = meshes;
            }

            {//ShaderGroups
                std::list<std::string> shaderGroups;
                for(auto sgm : s->shaderGroups)
                    shaderGroups.push_front(sgm.first);
                jScene["preloaded_resources"]["shaders"] = shaderGroups;
            }

            {//Cubemaps TODO: come back to...
                std::list<json> cubemaps;
                for(auto cm : s->cubemaps)
                {
                    json cmj;
                    cmj["tag"] = cm.first;
                    cmj["xpos"] = cm.second->xp_url;
                    cmj["xneg"] = cm.second->xn_url;

                    cmj["ypos"] = cm.second->yp_url;
                    cmj["yneg"] = cm.second->yn_url;

                    cmj["zpos"] = cm.second->zp_url;
                    cmj["zneg"] = cm.second->zn_url;

                    cubemaps.push_front(cmj);
                }

                jScene["preloaded_resources"]["cubemaps"] = cubemaps;
            }

            {//Materials
                std::list<std::string> materials;
                for(auto mm : s->materials)
                    materials.push_front(mm.second.url);
                jScene["preloaded_resources"]["materials"] = materials;
            }
        }


        json serializeSceneJson(Scene* s)
        {
            json jScene;

            jScene["version"]["major"] = JSCENE_V_MAJOR;
            jScene["version"]["minor"] = JSCENE_V_MINOR;

            jScene["skybox"] = s->skyboxCubemapName;

            serializeSceneJson_Materials(jScene, s);

            return jScene;
        }

        json serializeCurrentSceneJson()
        {
            Scene *s = Scene::currentScene;

            json jScene;

            jScene["version"]["major"] = JSCENE_V_MAJOR;
            jScene["version"]["minor"] = JSCENE_V_MINOR;

            jScene["skybox"] = s->skyboxCubemapName;

            serializeSceneJson_Materials(jScene, s);
            {
                std::list<json> ents;
                for(Entity* ent : *EntityManager::getAllEntities())
                {
                    if(ent!=nullptr)
                    {
                        for(auto t : ent->tags)
                            if(t->name=="no_export")
                                goto test_label;

                        ents.push_back(EntitySerial::serializeEntity(ent));
                    }
                    test_label:
                    continue;
                }
                jScene["entities"] = ents;
            }
            Log::scc("Serialized Scene.");

            return jScene;
        }

    }
}