#include <ge/engine/scene/EntityDeserializer.h>
#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentConstructorRegistry.h>
#include <ge/entity/EntityManager.h>
#include <ge/console/Log.h>

#include <glm/glm.hpp>

//NOTE: this shouldn't be here. maybe make the loader thing and io thing...
using json = nlohmann::json;
namespace ge
{
    namespace EntityDeserializer
    {
        const std::string LOG_TAG = "EntityDeserializer";
        std::list<Entity*> deserializeJson(json jEnts)
        {
            std::list<Entity*> ents;

            for(json e : jEnts)
            {
                Entity *ent = new Entity();
                ent->name = e["name"];
                ents.push_front(ent);
                for(json jCmp : e["components"])
                {
                    Component* cmp = ComponentConstructorRegistry::newComponent(jCmp["type"], ent);
                    ent->insertComponent(cmp);

                    for(json jVar : jCmp["vars"])
                    {
                        if(cmp->publicVars.count(jVar["name"]))
                        {
                            switch(cmp->publicVars[jVar["name"]].first) //TODO: REALLY DO A UTILITY FOR THIS BULLSHIT
                            {
                            case ge::DataType::FLOAT:
                                (*(float*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::INT:
                                (*(int*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::DOUBLE:
                                (*(double*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::BOOL:
                                (*(bool*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::STRING:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::MESH_DATA:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::MATERIAL:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::SHADER:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::SHADER_GROUP:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::TEXTURE:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::CUBEMAP:
                                (*(std::string*)cmp->publicVars[jVar["name"]].second) = jVar["value"];
                                break;
                            case ge::DataType::FVEC2:
                                (*(glm::vec2*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec2*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                break;
                            case ge::DataType::FVEC3:
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).z = jVar["value"]["z"];
                                break;
                            case ge::DataType::COLOUR3:
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                (*(glm::vec3*)cmp->publicVars[jVar["name"]].second).z = jVar["value"]["z"] ;
                                break;
                            case ge::DataType::FVEC4:
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).z = jVar["value"]["z"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).w = jVar["value"]["w"];
                                break;
                            case ge::DataType::COLOUR4:
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).z = jVar["value"]["z"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).w = jVar["value"]["w"];
                                break;
                            case ge::DataType::QUAT:
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).x = jVar["value"]["x"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).y = jVar["value"]["y"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).z = jVar["value"]["z"];
                                (*(glm::vec4*)cmp->publicVars[jVar["name"]].second).w = jVar["value"]["w"];
                                break;

                            case ge::DataType::FMAT2:
                                (*(glm::mat2*)cmp->publicVars[jVar["name"]].second)[0][0] = jVar["value"]["m00"];
                                (*(glm::mat2*)cmp->publicVars[jVar["name"]].second)[0][1] = jVar["value"]["m01"];
                                (*(glm::mat2*)cmp->publicVars[jVar["name"]].second)[1][0] = jVar["value"]["m10"];
                                (*(glm::mat2*)cmp->publicVars[jVar["name"]].second)[1][1] = jVar["value"]["m11"];
                                break;
                            case ge::DataType::FMAT3:
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[0][0] = jVar["value"]["m00"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[0][1] = jVar["value"]["m01"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[0][2] = jVar["value"]["m02"];
                                
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[1][0] = jVar["value"]["m10"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[1][1] = jVar["value"]["m11"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[1][2] = jVar["value"]["m12"];
                                
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[2][0] = jVar["value"]["m20"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[2][1] = jVar["value"]["m21"];
                                (*(glm::mat3*)cmp->publicVars[jVar["name"]].second)[2][2] = jVar["value"]["m22"];
                                break;

                            case ge::DataType::FMAT4:
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[0][0] = jVar["value"]["m00"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[0][1] = jVar["value"]["m01"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[0][2] = jVar["value"]["m02"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[0][3] = jVar["value"]["m03"];
                                
                                
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[1][0] = jVar["value"]["m10"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[1][1] = jVar["value"]["m11"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[1][2] = jVar["value"]["m12"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[1][3] = jVar["value"]["m13"];

                                
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[2][0] = jVar["value"]["m20"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[2][1] = jVar["value"]["m21"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[2][2] = jVar["value"]["m22"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[2][3] = jVar["value"]["m23"];

                                
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[3][0] = jVar["value"]["m30"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[3][1] = jVar["value"]["m31"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[3][2] = jVar["value"]["m32"];
                                (*(glm::mat4*)cmp->publicVars[jVar["name"]].second)[3][3] = jVar["value"]["m33"];
                                break;
                            }
                        }
                        else
                        {
                            Log::wrn(LOG_TAG, "Component type '"+cmp->getTypeName()+"' doesn't contain public value '"+jVar["name"].get<std::string>()+"', possibly corrupt Scene/Entity Data. SKIPPING");
                        }
                        
                    }
                }
            }
            return ents;
        }

        void activateEntities(std::list<Entity*> ents)
        {
            for(Entity* e : ents)
            {
                for(auto c : e->components)
                {
                    c.second->insertToDefaultBatch();
                }
                EntityManager::registerEntity(e);
            }
        }
    }
}