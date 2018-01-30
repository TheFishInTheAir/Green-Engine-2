#include <loader/LoadScene.h>
#include "util/ResourceUtil.h"
#include <json/json.hpp>
#include "loader/LoadImage.h"
#include "loader/LoadMesh.h"
#include "engine/empty_types/EmptyStaticObject.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

#include <memory/GlobalMemory.h>
#include "engine/scene/Scene.h"

using json = nlohmann::json;


namespace ge
{

	namespace SceneLoader
	{		
		Error loadSceneJson(std::string path, Empty::Scene* outScene)
		{	

			Empty::Scene scene;

			std::string file;
			ResourceUtil::getRawStrResource(path, &file);

			json data = json::parse(file.c_str());

			/**
			 *
			 * Resource Loading
			 *
			 */
			json res  = data["preloaded_resources"];

			bool hasCustomShaders = res.count("shaders") != 0;

			std::vector<std::string> textures = res["textures"];
			std::vector<std::string> meshes = res["meshes"];

			//std::vector<std::string> kept;
			
			ge::Scene* currentScene = GlobalMemory::get("ge_current_scene").getRawData<ge::Scene>();

			for(auto s : textures)
			{
				if(currentScene->textures.count(s)!=0)
				{
					scene.keptRes.push_back(s);
					continue;
				}
				Image *i;
				ImageLoader::loadImage(s, &i);

				scene.images.insert({ std::string(s), *i });

			}

			for(auto s : meshes)
			{

				if (currentScene->meshes.count(s) != 0)
				{
					scene.keptRes.push_back(s);
					continue;
				}

				Empty::MeshData md;
				MeshLoader::loadTriangleMesh(s, &md);
			
				scene.meshes.insert({ std::string(s), md });
			}

			if (hasCustomShaders) 
			{
				std::vector<std::string> shaders = res["shaders"];

				for (auto s : shaders)
				{

					if (currentScene->shaders.count(s) != 0)
					{
						scene.keptRes.push_back(s);
						continue;
					}

					scene.shaders.push_front( std::string(s) );
				}

			}

			/**
			*
			* Static Object
			*
			*/

			std::vector<json> static_objects = data["static_objects"];

			for (json object : static_objects)
			{
				Empty::StaticObject obj;

				glm::mat4 model(1);

				if (object["model"].count("pos_x") != 0)
					model = glm::translate(model, { (float)object["model"]["pos_x"], (float)object["model"]["pos_y"], (float)object["model"]["pos_z"] });
				if (object["model"].count("scale_x") != 0)
					model = glm::scale(model, { (float)object["model"]["scale_x"], (float)object["model"]["scale_y"], (float)object["model"]["scale_z"] });
				if (object["model"].count("rot_x") != 0)
				{
					glm::quat r = { (float)object["model"]["rot_x"], (float)object["model"]["rot_y"], (float)object["model"]["rot_z"], (float)object["model"]["rot_w"] };
					model *= glm::toMat4(r);


				}
				
				if (object.count("uv_scale") != 0)
				{
					obj.uvscale = (float) object["uv_scale"];
				}
				else
				{
					obj.uvscale = 1;
				}
				obj.model.matrix = model;

				json material = object["material"];
				{
					std::string string = material["lighting_type"];
					obj.lighting_type = string;
				}

				obj.override_tex = material["override_tex"];

				

				if(material.count("custom_shader")!=0)
				{
					std::string string = material["custom_shader"];

					obj.custom_shader = string;
				}
				else
				{
					obj.custom_shader = "";
				}


				//Textures


				if (material.count("albedo") != 0)
				{
					std::string string = object["material"]["albedo"];

					obj.albedo = string;
				}
				else
				{
					obj.albedo = "";
				}

				if (material.count("specular") != 0)
				{
					std::string string = material["specular"];

					obj.specular = string;
				}
				else
				{
					obj.specular = "";
				}

				if (material.count("normal") != 0)
				{
					std::string string = material["normal"];

					obj.normal = string;
				}
				else
				{
					obj.normal = "";
				}

				if (material.count("roughness") != 0)
				{
					std::string string = material["roughness"];

					obj.roughness = string;
				}
				else
				{
					obj.roughness = "";
				}

				std::string string_temp = object["mesh"];

				obj.mesh = string_temp;


				scene.staticObjects.push_front(obj);
			}

			*outScene = scene;

			return Error();

		}

	}
}
