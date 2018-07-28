#include <ge/loader/LoadMaterial.h>
#include <ge/util/ResourceUtil.h>
#include <ge/console/Log.h>
#include <json/json.hpp>
#include <unordered_map>
using json = nlohmann::json;


#define DEF_GS_TABLE
#include <engine/global_settings.pre>
//#define DEF_GS_TABLE
//#include <engine/global_settings.pre>



namespace ge
{
    namespace MaterialLoader
    {

        const std::unordered_map<std::string, int> INTERFACE_TYPE_LOOKUP = 
        {
            {"tex2d",   0},
            {"tex3d",   1},
            {"float",   2},
            {"int",     3},
            {"vec2",    4},
            {"vec3",    5},
            {"vec4",    6},
            {"mat2",    7},
            {"mat3",    8},
            {"mat4",    9}
            
        };


        const std::string LOG_TAG = "MaterialLoader";
         
        void errIfNotExist(std::string matPath, json jobj, std::string str)
        {
            if(!jobj.count(str))
            {
                Log::critErr(LOG_TAG, "Corrupt Material '"+matPath+"': Does not Contain json value '"+str+"'");
            }
        }

        std::string getGSValueIfExist(std::string v)
        {
            if(ge_global_settings::map.count(v)!=0)
                return ge_global_settings::map[v];
            else
                return v;
        }

        Material loadMaterial(std::string url, bool isResource)
        {
            Material mat;

            //Read Data
			std::string file;
			if (isResource)
				ResourceUtil::getRawStrResource(url, &file);
			else
				ResourceUtil::getRawStrFile(url, &file);
			

            //Json Parsing
			json data = json::parse(file.c_str());

            //Version
            if(!data.count("version"))
                Log::wrn(LOG_TAG, "No Material Format Version Given! Assuming latest.");
            else
            {   //TODO: Make this do something
                std::string vrs("");
                vrs += std::to_string((int)data["version"]["major"]);
                vrs += ".";
                vrs += std::to_string((int)data["version"]["minor"]);
                
                Log::vrb(LOG_TAG, "Material Format Version: "+vrs);
            }


            errIfNotExist(url, data, "name");
            mat.name = data["name"];

            errIfNotExist(url, data, "shader_group");
            mat.shader_group = data["shader_group"];

            mat.url = url;
            //NOTE: we might be able to combine the interface setup and interface populating stages...
            //Interface Setup

            //                 disp_name             data_type    meta data
            std::unordered_map<std::string, std::pair<int, std::string>> interface;
            {
                std::string file;
                ResourceUtil::getRawStrResource(mat.shader_group, &file);
                json sgData = json::parse(file.c_str());

                for(auto j : sgData["interface"])
                {
                    switch(INTERFACE_TYPE_LOOKUP.at(j["type"].get<std::string>())) //TODO: add all of the other varients
                    {
                        case 0: //TEX2D
                            //Log::scc(j["tex2d_slot"].get<std::string>());
                            interface[j["disp_name"]] = {0,
                                    j["tex2d_slot"].is_string() ? ge_global_settings::map[j["tex2d_slot"].get<std::string>()] : j["tex2d_slot"].get<std::string>()};
                            break;
                        case 2: //FLOAT
                            interface[j["disp_name"]] = {2, getGSValueIfExist(j["u_pos"])};
                            break;
                    }
                }
            }

            //Interface Population
            for(auto value : data["values"])
            {
                if(interface.count(value["disp_name"]))
                {
                    switch(interface[value["disp_name"]].first) //TODO: add all of the other varients
                    {
                        case 0: //TEX2D     
                            //Log::dbg(interface[value["disp_name"]].second);    
                            mat.textures.push_back({value["disp_name"], {value["value"].get<std::string>(), std::stoi(interface[value["disp_name"]].second)}});
                            break;
                    }
                }
                else
                    Log::critErr(LOG_TAG, std::string()+"Shader Manifest Interface doesn't contain disp_name '"+value["disp_name"].get<std::string>()+"', possibly corrupt ShaderManifest/Material.");
            }
            return mat;
        }

    }
}