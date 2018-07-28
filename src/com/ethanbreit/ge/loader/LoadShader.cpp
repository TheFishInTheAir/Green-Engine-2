//
// Created by Ethan Breit on 2017-12-23.
//
#include <ge/loader/LoadShader.h>

#define DEF_GS_TABLE
#define DEF_GS_TABLE_IMPLEMENTATION //Arbitrary placement of implementation...
#include <engine/global_settings.pre>


#include <ge/util/ResourceUtil.h>
#include "json/json.hpp"
#include <ge/graphics/types/Shader.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/engine/scene/Scene.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/console/Log.h>
#include <unordered_map>




#ifdef _WIN32

#define DIR_SEP '\\'

#else

#define DIR_SEP '/'

#endif

namespace ge
{
    const std::string LOG_TAG = "ShaderLoader";
    namespace ShaderLoader
    {


        std::vector<std::shared_ptr<Shader>> loadManifest(nlohmann::json, Scene*);
        std::unordered_map<std::string, Uniform> loadUniforms(nlohmann::json);
        std::vector<std::shared_ptr<Shader>> loadMrgManifest(nlohmann::json, Scene*);
        std::vector<std::shared_ptr<Shader>> loadNrmManifest(nlohmann::json, Scene*);
        

        Error loadShader(std::string path, ShaderGroup** outShaderGroup)
        {
            static GraphicsCore* gc = GraphicsCore::ctx;

            Log::vrb(LOG_TAG, "Loading shader :"+path);

			nlohmann::json manifest;

			{
				std::string data;
				ResourceUtil::getRawStrResource(path, &data);
				manifest = nlohmann::json::parse(data.c_str());
			}

            std::vector<std::shared_ptr<Shader>> shaders = loadManifest(manifest, Scene::currentScene);

			ShaderGroup *shaderGroup;

			gc->shaderFactory->genShaderGroup(shaders, &shaderGroup);

			*outShaderGroup = shaderGroup;

            return Error();
        }

	    Error loadShader(std::string path, ShaderGroup** outShaderGroup, Scene* s)
	    {
            static GraphicsCore* gc = GraphicsCore::ctx;

            Log::vrb(LOG_TAG, "Loading shader :" + path + "\n");

			nlohmann::json manifest;

			{
				std::string data;
				ResourceUtil::getRawStrResource(path, &data);
				manifest = nlohmann::json::parse(data.c_str());
			}

			std::vector<std::shared_ptr<Shader>> shaders = loadManifest(manifest, s);

			ShaderGroup *shaderGroup;

			gc->shaderFactory->genShaderGroup(shaders, &shaderGroup);

            shaderGroup->uniforms = loadUniforms(manifest);
            
			*outShaderGroup = shaderGroup;

			return Error();
	    }
        
        Uniform genRightUniformFromType(std::string type)
        {
            if(type=="float")
                return Uniform(Uniform::FLOAT);
            else if(type == "int")
                return Uniform(Uniform::INT);
            else if(type == "vec2")
                return Uniform(Uniform::FVEC2);
            else if(type == "vec3")
                return Uniform(Uniform::FVEC3);
            else if(type == "vec4")
                return Uniform(Uniform::FVEC4);
            else if(type == "mat2")
                return Uniform(Uniform::MAT2);
            else if(type == "mat3")
                return Uniform(Uniform::MAT3);
            else if(type == "mat4")
                return Uniform(Uniform::MAT4);
            
            
            return Uniform(Uniform::INT);

        }
        
        std::unordered_map<std::string, Uniform> loadUniforms(nlohmann::json jsn)
        {
            std::unordered_map<std::string, Uniform> uniforms;
            
            std::string prefix = "";
            if(jsn.count("_prefix"))
                prefix = jsn["_prefix"];
            for(auto uni : jsn["uniforms"])
            {
                std::string name = uni["name"];
                if(ge_global_settings::map.count(name))
                    name = ge_global_settings::map.at(name);

                name = prefix+name;

                if(uni["type"]=="struct_array")
                {

                    int array_size = 0;
                    if(uni["array_size"].is_string())
                        array_size = std::stoi(ge_global_settings::map.at(uni["array_size"]));
                    else
                        array_size = uni["array_size"];

                    uniforms.insert({name,Uniform(Uniform::INT)});

                    for(int i = 0; i < array_size; i++)
                    {
                        std::string localPrefix = name+"["+std::to_string(i)+"]."; //NOTE: REMEMBER THE PERIOD!!!!
                        nlohmann::json localJsn = uni["struct_members"];

                        localJsn["_prefix"] = localPrefix;
                        //Log::dbg("test23");

                        std::unordered_map<std::string, Uniform> localUniforms = loadUniforms(localJsn);

                        for(auto su : localUniforms)
                        {
                            //Log::critErr(su.first);
                            uniforms.insert(su);
                        }
                    }
                }

                Log::dbg(name);
                Uniform u = genRightUniformFromType(uni["type"]);
                uniforms.insert({name, u});
            }
            
            return uniforms;
        }
        
        std::vector<std::shared_ptr<Shader>> loadManifest(nlohmann::json jsn, Scene* scn)
        {
            std::vector<std::shared_ptr<Shader>> shaders;
            
            if(jsn.count("merge"))
                return loadMrgManifest(jsn, scn);
            else
                return loadNrmManifest(jsn, scn);
                
        }
        
        std::vector<std::shared_ptr<Shader>> loadNrmManifest(nlohmann::json manifest, Scene* s)
        {
            std::vector<std::shared_ptr<Shader>> shaders;
            
            GraphicsCore* gc = GraphicsCore::ctx;
            
            if (manifest.count("vertex"))
            {
                if(s->shaders.count(manifest["vertex"])>0)
                {
                    shaders.push_back(s->shaders.at(manifest["vertex"]));
                }
                else
                {
                    std::shared_ptr<Shader> vert;
                    
                    std::string vertData;
                    ResourceUtil::getRawStrResource(manifest["vertex"], &vertData);
                    gc->shaderFactory->genShader(vertData, ShaderType::Shader_Vertex, &vert);
                    
                    
                    s->shaders.insert({manifest["vertex"].get<std::string>(), vert});
                    
                    shaders.push_back(vert);
                }
            }
            
            
            
            if (manifest.count("geometry"))
            {
                if (s->shaders.count(manifest["geometry"])>0)
                {
                    shaders.push_back(s->shaders.at(manifest["geometry"]));
                }
                else
                {
                    std::shared_ptr<Shader> geom;
                    
                    std::string geomData;
                    ResourceUtil::getRawStrResource(manifest["geometry"], &geomData);
                    gc->shaderFactory->genShader(geomData, ShaderType::Shader_Geometry, &geom);
                    
                    s->shaders.insert({manifest["geometry"].get<std::string>(), geom });
                    
                    
                    shaders.push_back(geom);
                }
            }
            
            
            if (manifest.count("fragment"))
            {
                if (s->shaders.count(manifest["fragment"])>0)
                {
                    shaders.push_back(s->shaders.at(manifest["fragment"]));
                }
                else
                {
                    std::shared_ptr<Shader> frag;
                    
                    std::string fragData;
                    ResourceUtil::getRawStrResource(manifest["fragment"], &fragData);
                    gc->shaderFactory->genShader(fragData, ShaderType::Shader_Fragment, &frag);
                    
                    s->shaders.insert({manifest["fragment"].get<std::string>(), frag });
                    
                    shaders.push_back(frag);
                }
            }

            
            return shaders;
        }
        
        std::vector<std::shared_ptr<Shader>> loadMrgManifest(nlohmann::json jsn, Scene* scn)
        {
            std::vector<std::shared_ptr<Shader>> shaders;
            
            std::string shaderURL = jsn["merge"];
            
            std::vector<std::string> shaderTypes = jsn["merge_info"];
            
            for(std::string shaderType : shaderTypes)
            {
                if(shaderType=="VERTEX")
                {
                    if (scn->shaders.count(shaderURL+"_VERT")>0)
                    {
                        shaders.push_back(scn->shaders.find(shaderURL+"_VERT")->second);
                    }
                    else
                    {
                        std::shared_ptr<Shader> frag;
                        
                        std::string fragData;
                        ResourceUtil::getRawStrResource(shaderURL, &fragData);
                        
                        fragData = fragData.substr(0,fragData.find_first_of('\n')+1)+
                        "\n\n#define VERTEX\n\n\n"+
                        fragData.substr(fragData.find_first_of('\n')+1, fragData.length());
                        
                        GraphicsCore::ctx->shaderFactory->genShader(fragData, ShaderType::Shader_Vertex, &frag);
                        
                        scn->shaders.insert({shaderURL+"_VERT", frag });
                        
                     
                        shaders.push_back(frag);
                    }

                }
                else if(shaderType=="GEOMETRY")
                {
                    if (scn->shaders.count(shaderURL+"_GEOM")>0)
                    {
                        shaders.push_back(scn->shaders.find(shaderURL+"_GEOM")->second);
                    }
                    else
                    {
                        std::shared_ptr<Shader> geom;
                        
                        std::string geomData;
                        ResourceUtil::getRawStrResource(shaderURL, &geomData);
                        
                        geomData = geomData.substr(0,geomData.find_first_of('\n')+1)+
                        "#define GEOMETRY"+
                        geomData.substr(geomData.find_first_of('\n')+1, geomData.length());
                        
                        GraphicsCore::ctx->shaderFactory->genShader(geomData, ShaderType::Shader_Geometry, &geom);
                        
                        scn->shaders.insert({shaderURL+"_GEOM", geom });
                        
                        shaders.push_back(geom);
                    }
                }
                else if(shaderType=="FRAGMENT")
                {
                    if (scn->shaders.count(shaderURL+"_FRAG")>0)
                    {
                        shaders.push_back(scn->shaders.find(shaderURL+"_FRAG")->second);
                    }
                    else
                    {
                        std::shared_ptr<Shader> frag;
                        
                        std::string fragData;
                        ResourceUtil::getRawStrResource(shaderURL, &fragData);
                        
                        fragData = fragData.substr(0,fragData.find_first_of('\n')+1)+
                        "#define FRAGMENT"+
                        fragData.substr(fragData.find_first_of('\n')+1, fragData.length());
                        
                        GraphicsCore::ctx->shaderFactory->genShader(fragData, ShaderType::Shader_Fragment, &frag);
                        
                        scn->shaders.insert({shaderURL+"_FRAG", frag });
                        
                        shaders.push_back(frag);
                    }
                }
            }
            
            return shaders;
        }
    }
}
