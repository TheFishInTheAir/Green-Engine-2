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

namespace ge
{
    struct Scene //TODO: think before writing
    {

		//TODO: File that contains scene data that can load textures, multiple objects etc.

		//TODO: UUID for each file so its not loaded twice 

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

		std::forward_list<std::shared_ptr<ge::Image>>   images;
		std::forward_list<std::shared_ptr<ge::Texture>> textures;
		//std::map<std::string, std::shared_ptr<ge::Empty::MeshData>> meshs;
		//std::map<std::string, std::shared_ptr<ge::Shader>> shaders;
		//std::map<std::string, std::shared_ptr<ge::ShaderGroup>> shaderGroups;

		//TODO: create light class/struct 

		//std::vector<Light> lights;

	};
}
