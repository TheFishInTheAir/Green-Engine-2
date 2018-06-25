#include <ge/loader/LoadScene.h>
#include <ge/util/ResourceUtil.h>
#include <json/json.hpp>
#include <ge/loader/LoadImage.h>
#include <ge/loader/LoadMesh.h>
#include <ge/engine/empty_types/EmptyStaticObject.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include <ge/memory/GlobalMemory.h>
#include <ge/engine/scene/Scene.h>
using json = nlohmann::json;


namespace ge
{

	namespace SceneLoader
	{		
		Error loadSceneJson(std::string path, Empty::Scene* outScene, bool isResource)
		{	

			Empty::Scene scene;

			ConsoleIO::print("Loading Scene: "+path+"\n");


			std::string file;
			if (isResource)
				ResourceUtil::getRawStrResource(path, &file);
			else
				ResourceUtil::getRawStrFile(path, &file);
			json data = json::parse(file.c_str());

			ConsoleIO::print("Version Number (not included yet)\n");

			/**
			 *
			 * Resource Loading
			 *
			 */
			json res  = data["preloaded_resources"];

			bool hasCustomShaders = res.count("shaders") != 0;
			bool hasCubeMaps = res.count("cubemaps") != 0;

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

			//Tagged Resources

			if (hasCubeMaps)
			{
				std::vector<json> cubemaps = res["cubemaps"];
				for (auto cmap : cubemaps)
				{

					if (cmap.count("xpos"))
					{
						ge::Image* xpos;
						ge::Image* xneg;

						ge::Image* ypos;
						ge::Image* yneg;

						ge::Image* zpos;
						ge::Image* zneg;


						ImageLoader::loadImage(cmap["xpos"], &xpos);
						ImageLoader::loadImage(cmap["xneg"], &xneg);

						ImageLoader::loadImage(cmap["ypos"], &ypos);
						ImageLoader::loadImage(cmap["yneg"], &yneg);

						ImageLoader::loadImage(cmap["zpos"], &zpos);
						ImageLoader::loadImage(cmap["zneg"], &zneg);

						scene.cubemaps.insert({ cmap["tag"],{ *xpos,*xneg, *ypos,*yneg, *zpos,*zneg } });

					}else
					{
						ConsoleIO::print("Invalid Cubemap syntax, must give image coordinates.\n", MessageType::Warning);
					}
				}
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
			* SkyBox
			*/

			if(data.count("skybox")!=0)
			{
				scene.skybox = data["skybox"]; // Not an error
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

				ModelMat model;

				model.scale.x = object["model"].count("scale_x") ? object["model"]["scale_x"] : 1;
				model.scale.y = object["model"].count("scale_y") ? object["model"]["scale_y"] : 1;
				model.scale.z = object["model"].count("scale_z") ? object["model"]["scale_z"] : 1;

				model.pos.x = object["model"].count("pos_x") ? object["model"]["pos_x"] : 0;
				model.pos.y = object["model"].count("pos_y") ? object["model"]["pos_y"] : 0;
				model.pos.z = object["model"].count("pos_z") ? object["model"]["pos_z"] : 0;

				model.rot.x = object["model"].count("rot_x") ? object["model"]["rot_x"] : 0;
				model.rot.y = object["model"].count("rot_y") ? object["model"]["rot_y"] : 0;
				model.rot.z = object["model"].count("rot_z") ? object["model"]["rot_z"] : 0;
				model.rot.w = object["model"].count("rot_w") ? object["model"]["rot_w"] : 0;

				if (object.count("uv_scale") != 0)
				{
					obj.uvscale = (float) object["uv_scale"];
				}
				else
				{
					obj.uvscale = 1;
				}
				obj.model = model;

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
