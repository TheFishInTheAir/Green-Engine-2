//
// Created by Ethan Breit on 2017-08-15.
//

#pragma once
#include <forward_list>
#include <ge/graphics/types/Texture.h>
#include <ge/graphics/empty_types/MeshData.h>
#include <ge/graphics/types/Shader.h>
#include <memory>
#include <ge/graphics/types/Image.h>
#include <unordered_map>
#include <ge/graphics/empty_types/Scene.h>
#include <ge/graphics/meshes/TriangleMesh.h>
#include <ge/engine/defaults/SkyBox.h>


namespace ge
{


	struct Scene
    {

		static Scene* currentScene = nullptr;

		void loadScene(Empty::Scene);

		void instantiateScene();

		static void init();

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

		std::unordered_map<std::string, std::shared_ptr<ge::Texture>>		textures;
		std::unordered_map<std::string, std::shared_ptr<ge::CubeMap>>		cubemaps;

		std::unordered_map<std::string, std::shared_ptr<ge::Empty::MeshData>>	meshes;
		std::unordered_map<std::string, std::shared_ptr<ge::ShaderGroup>>		shaderGroups;
		std::unordered_map<std::string, std::shared_ptr<ge::Shader>>			shaders;


		std::forward_list<void*>											SuperSketchyEntityHandler; //Should fix this I think I might of actually...

		

		//TODO: create light class/struct @UNFINISHED

		//std::vector<Light> lights;

	};
}
