#include <ge/engine/scene/SceneSerializer.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/Component.h>
#include <glm/ext.hpp>

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
                    textures.push_front(tm.first);
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

        void serializeSceneJson_Entities(json &jScene, Scene* s) //1.3
        {
            std::vector<Entity*>* ents = EntityManager::getAllEntities();

            std::list<json> jEnts;
            for(Entity* e : *ents)
            {
                json j;
                j["name"] = e->name;

                std::list<json> jCmps;
                for(auto cm : e->components)
                {
                    json cmp;
                    cmp["type"] = cm.first;

                    std::list<json> jVars;
                    for(auto v : cm.second->publicVars)
                    {
                        json jv;

                        jv["name"] = v.first;

                        switch(v.second.first) //TODO: create utility for this
                        {
                            case ge::DataType::FLOAT:
                                jv["value"] = *(float*)v.second.second;
                                break;
                            case ge::DataType::INT:
                                jv["value"] = *(int*)v.second.second;
                                break;
                            case ge::DataType::DOUBLE:
                                jv["value"] = *(double*)v.second.second;
                                break;
                            case ge::DataType::BOOL:
                                jv["value"] = *(bool*)v.second.second;
                                break;
                            case ge::DataType::STRING:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::MESH_DATA:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::MATERIAL:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::SHADER:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::SHADER_GROUP:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::TEXTURE:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::CUBEMAP:
                                jv["value"] = *(std::string*)v.second.second;
                                break;
                            case ge::DataType::FVEC2:
                                jv["value"]["x"] = (*(glm::vec2*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::vec2*)v.second.second).y;
                                break;
                            case ge::DataType::FVEC3:
                                jv["value"]["x"] = (*(glm::vec3*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::vec3*)v.second.second).y;
                                jv["value"]["z"] = (*(glm::vec3*)v.second.second).z;
                                break;
                            case ge::DataType::COLOUR3:
                                jv["value"]["x"] = (*(glm::vec3*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::vec3*)v.second.second).y;
                                jv["value"]["z"] = (*(glm::vec3*)v.second.second).z;
                                break;
                            case ge::DataType::FVEC4:
                                jv["value"]["x"] = (*(glm::vec4*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::vec4*)v.second.second).y;
                                jv["value"]["z"] = (*(glm::vec4*)v.second.second).z;
                                jv["value"]["w"] = (*(glm::vec4*)v.second.second).w;
                                break;
                            case ge::DataType::COLOUR4:
                                jv["value"]["x"] = (*(glm::vec4*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::vec4*)v.second.second).y;
                                jv["value"]["z"] = (*(glm::vec4*)v.second.second).z;
                                jv["value"]["w"] = (*(glm::vec4*)v.second.second).w;
                                break;
                            case ge::DataType::QUAT:
                                jv["value"]["x"] = (*(glm::quat*)v.second.second).x;
                                jv["value"]["y"] = (*(glm::quat*)v.second.second).y;
                                jv["value"]["z"] = (*(glm::quat*)v.second.second).z;
                                jv["value"]["w"] = (*(glm::quat*)v.second.second).w;
                                break;

                            case ge::DataType::FMAT2:
                                jv["value"]["m00"] = (*(glm::mat2*)v.second.second)[0][0];
                                jv["value"]["m01"] = (*(glm::mat2*)v.second.second)[0][1];
                                jv["value"]["m10"] = (*(glm::mat2*)v.second.second)[1][0];
                                jv["value"]["m11"] = (*(glm::mat2*)v.second.second)[1][1];
                                break;
                            case ge::DataType::FMAT3:
                                jv["value"]["m00"] = (*(glm::mat3*)v.second.second)[0][0];
                                jv["value"]["m01"] = (*(glm::mat3*)v.second.second)[0][1];
                                jv["value"]["m02"] = (*(glm::mat3*)v.second.second)[0][2];
                                
                                jv["value"]["m10"] = (*(glm::mat3*)v.second.second)[1][0];
                                jv["value"]["m11"] = (*(glm::mat3*)v.second.second)[1][1];
                                jv["value"]["m12"] = (*(glm::mat3*)v.second.second)[1][2];

                                jv["value"]["m20"] = (*(glm::mat3*)v.second.second)[2][0];
                                jv["value"]["m21"] = (*(glm::mat3*)v.second.second)[2][1];
                                jv["value"]["m22"] = (*(glm::mat3*)v.second.second)[2][2];
                                break;

                            case ge::DataType::FMAT4:
                                jv["value"]["m00"] = (*(glm::mat4*)v.second.second)[0][0];
                                jv["value"]["m01"] = (*(glm::mat4*)v.second.second)[0][1];
                                jv["value"]["m02"] = (*(glm::mat4*)v.second.second)[0][2];
                                jv["value"]["m03"] = (*(glm::mat4*)v.second.second)[0][3];
                                
                                jv["value"]["m10"] = (*(glm::mat4*)v.second.second)[1][0];
                                jv["value"]["m11"] = (*(glm::mat4*)v.second.second)[1][1];
                                jv["value"]["m12"] = (*(glm::mat4*)v.second.second)[1][2];
                                jv["value"]["m13"] = (*(glm::mat4*)v.second.second)[1][3];

                                jv["value"]["m20"] = (*(glm::mat4*)v.second.second)[2][0];
                                jv["value"]["m21"] = (*(glm::mat4*)v.second.second)[2][1];
                                jv["value"]["m22"] = (*(glm::mat4*)v.second.second)[2][2];
                                jv["value"]["m23"] = (*(glm::mat4*)v.second.second)[2][3];

                                jv["value"]["m30"] = (*(glm::mat4*)v.second.second)[3][0];
                                jv["value"]["m31"] = (*(glm::mat4*)v.second.second)[3][1];
                                jv["value"]["m32"] = (*(glm::mat4*)v.second.second)[3][2];
                                jv["value"]["m33"] = (*(glm::mat4*)v.second.second)[3][3];
                                break;
                        }

                        jVars.push_front(jv);
                    }
                    cmp["vars"] = jVars;
                    jCmps.push_front(cmp);
                }
                j["components"] = jCmps;
                jEnts.push_front(j);
            }
            jScene["entities"] = jEnts;
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
            serializeSceneJson_Entities(jScene, s);

            return jScene;
        }

    }
}