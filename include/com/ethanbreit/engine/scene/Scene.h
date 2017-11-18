//
// Created by Ethan Breit on 2017-08-15.
//

#pragma once
#include <forward_list>
#include "graphics/types/Texture.h"
#include "graphics/empty_types/MeshData.h"
#include "graphics/types/Shader.h"
#include <memory>
#include "graphics/types/Image.h"
#include <unordered_map>
#include "graphics/empty_types/Scene.h"
namespace ge
{


	struct Scene //TODO: think before writing
    {

		void loadScene(Empty::Scene);

		//Contains:
    	/*
    	 * Textures
    	 * MeshData
    	 * Shaders
    	 * ShaderGroups
		 * ??Images??
		 * 
		 * Sounds (in the future)
		 * 
    	 */

		//TODO: First get Textures working and then get the others going.

		std::unordered_map<std::string, std::shared_ptr<ge::Texture>> textures;
		std::unordered_map<std::string, std::shared_ptr<ge::Empty::MeshData>> meshs;
		std::unordered_map<std::string, std::shared_ptr<ge::ShaderGroup>> shaders;

		//TODO: create light class/struct 

		//std::vector<Light> lights;

	};
}
