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
#include <ge/graphics/types/lights/LightDirectional.h>
#include <ge/graphics/types/lights/LightPoint.h>
#include <ge/graphics/types/Material.h>


namespace ge
{


	struct Scene
    {

        static Scene* currentScene;

		void loadScene(Empty::Scene);

		void instantiateScene();

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
		std::unordered_map<std::string, ge::Material>						materials;

		std::unordered_map<std::string, std::shared_ptr<ge::ShaderGroup>>		shaderGroups;
		std::unordered_map<std::string, std::shared_ptr<ge::Shader>>			shaders;
        
		std::forward_list<Entity*>                                          uninstantiatedEntities; //Should fix this
        
        Entity* skybox;
		std::string skyboxCubemapName = "";

		//TODO: create light class/struct @UNFINISHED

		std::vector<LightDirectional*> 	directionalLights;
		std::vector<LightPoint*> 	pointLights;

	};
}
