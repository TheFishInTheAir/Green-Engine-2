#pragma once
#include <ge/engine/types/ModelMat.h>
#include <ge/engine/empty_types/EmptyStaticObject.h>

#include <ge/entity/Entity.h>

#include <engine/global_settings.pre>
#include <ge/graphics/meshes/TriangleMesh.h>
#include <memory>
#include <ge/runtime/Runtime.h>
#include <ge/graphics/Camera.h>
#include <ge/graphics/types/lights/LightDirectional.h>
#include <ge/graphics/GraphicsCore.h>

//TODO: fix textures, we don't need each Texture passed to us seperatly

namespace ge
{

	struct Scene;
    namespace StandardEntGen
    {
        Entity* genStaticObject(Empty::StaticObject);
    }
	//Everything below is pre 2.2 and is @DEPRECATED




	struct StaticObject
	{

		StaticObject(Empty::StaticObject);

		StaticObject(Empty::StaticObject, Scene);

		~StaticObject();

		ModelMat model;

		TriangleMesh* mesh;

		ShaderGroup* shaders;


		Texture* albedo;

		//std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> specular;
		std::shared_ptr<Texture> normal;
		std::shared_ptr<Texture> roughness;

		void render();

		void clear();

		ge_START_CYCLE_HANDLER(StaticObject)
			ge_GENERATE_TRAMPOLINE(render, RENDER)
		ge_END_CYCLE_HANDLER

	private:

		Uniform* u_model;
		Uniform* u_vp;
		Uniform* u_camPos;

		Camera* currentCamera;
		
		void initRenderer(Empty::StaticObject sobj);
		void initTextures(Empty::StaticObject*);

		static GraphicsCore* _gCore;


		//Temporary testing code

		Uniform* u_albedo;
		Uniform* u_specular;

		Uniform* u_time;
		float t = 0;

		bool isDefault = true;

		//Uniform* u_specular;


		Uniform* u_testlight_col;
		Uniform* u_testlight_dir;
		
		LightDirectional* testlight;

		void initTest();


	};
}
