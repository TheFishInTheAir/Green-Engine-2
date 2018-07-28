#pragma once

#include <forward_list>
#include <ge/graphics/types/Texture.h>
#include <ge/graphics/empty_types/MeshData.h>
#include <ge/graphics/types/Shader.h>
#include <memory>
#include <ge/graphics/types/Image.h>
#include <unordered_map>
#include <ge/engine/empty_types/EmptyStaticObject.h>
#include <array>
#include <ge/graphics/types/Material.h>
namespace ge
{
	namespace Empty
	{

		struct Scene 
		{


			std::vector<std::string>								keptRes;

			std::unordered_map<std::string, ge::Image>				images;
			std::unordered_map<std::string, ge::Empty::MeshData>	meshes;
			std::unordered_map<std::string, std::array<ge::Image, 6>>		cubemaps;


			std::forward_list<std::string>							shaders;
			std::forward_list<Material>							materials;

			std::forward_list<Empty::StaticObject>					staticObjects;


			std::string	skybox = "";
			//TODO: create light class/struct  @UNFINISHED

			//std::vector<Light> lights;

		};
	}
}
