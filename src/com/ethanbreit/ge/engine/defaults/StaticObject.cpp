#include <ge/engine/defaults/StaticObject.h>
#include <thread>
#include <ge/memory/GlobalMemory.h>
#include <ge/engine/scene/Scene.h>
#include <ge/runtime/Runtime.h>
#include <ge/console/ConsoleIO.h>
#include <ge/util/ResourceUtil.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/Component.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/entity/component/ComponentManager.h>
#include <ge/console/Log.h>
#include <ge/graphics/abs/OpenGL/types/GLShaderGroup.h>
namespace ge
{
	GraphicsCore* StaticObject::_gCore;

	StaticObject::StaticObject(Empty::StaticObject eso)
	{
		//Init Trampoline

		//ge_REGISTER_RUNTIME_HANDLER;

		_gCore = GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getRawData<GraphicsCore>();
		


		//get current scene

		ge::Scene s = GlobalMemory::get("ge_current_scene").getData<Scene>();


		if (std::this_thread::get_id() != GlobalMemory::get("ge_render_context_runtime").getData<Runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread", MessageType::Error);
		}

		currentCamera = GlobalMemory::get(DBL_STRINGIFY(CURRENT_CAMERA)).getRawData<Camera>();

		//testlight = GlobalMemory::get("testlight").getRawData<LightDirectional>();

		model = eso.model;
		mesh  = _gCore->meshFactory->newTriangleMesh(*s.meshes.find(eso.mesh)->second.get()); //TODO: fix to be shared pointer
		
		
		float* f = ((float*)mesh->getVertexBuffer("texcoords")->data->data);
		
		for (int i = 0; i < mesh->getVertexBuffer("texcoords")->data->size / sizeof(float); i++)
		{

			//ConsoleIO::print(std::to_string(f[i]) + "\n");
			(f[i]) *= eso.uvscale;

		}
		mesh->getVertexBuffer("texcoords")->bufferData();
		

		albedo = eso.albedo.empty() ? nullptr : s.textures.find(eso.albedo)->second.get();
		specular = eso.specular == "" ? nullptr : s.textures.find(eso.specular)->second;
		normal = eso.normal == "" ? nullptr : s.textures.find(eso.normal)->second;
		roughness = eso.roughness == "" ? nullptr : s.textures.find(eso.roughness)->second;

		albedo->setFiltering(TextureFilterType::Anisotropic_16x); 
		specular->setFiltering(TextureFilterType::Anisotropic_16x);


		initRenderer(eso);
		initTest();


		mesh->rebuffer();

		//Register to trampoline

		//GlobalMemory::get("ge_render_runtime_group").getRawData<RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this); 
	}
	void StaticObject::render()
	{
		t += 0.01f;
		/*u_vp->setData((currentCamera->vp));
		u_model->setData(model.getMat());

		u_camPos->setData(currentCamera->pos);
		u_time->setData(t);

		if (isDefault)
		{
			u_testlight_dir->setData(testlight->dir);
			u_testlight_col->setData(testlight->colour);
			u_albedo->setData(0);
			u_specular->setData(1);
		}*/
		mesh->render();
	}

	void StaticObject::clear()
	{

		GlobalMemory::get("ge_render_runtime_group").getRawData<RuntimeGroup>()->ge_RUNTIME_GROUP_REMOVE_HEAP(this);
		delete mesh;
	}


	StaticObject::StaticObject(Empty::StaticObject eso, ge::Scene s)
	{
		//Check if on Render Thread.

		if(std::this_thread::get_id()!=GlobalMemory::get("ge_render_context_runtime").getData<Runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread" , MessageType::Error);
		}


		model = eso.model;

		albedo		= eso.albedo	== "" ? nullptr : s.textures.find(eso.albedo)	->second.get();
		specular	= eso.specular	== "" ? nullptr : s.textures.find(eso.specular)	->second;
		normal		= eso.normal	== "" ? nullptr : s.textures.find(eso.normal)	->second;
		roughness	= eso.roughness == "" ? nullptr : s.textures.find(eso.roughness)->second;



	}

	StaticObject::~StaticObject()
	{
		clear();
	}


	void StaticObject::initRenderer(Empty::StaticObject sobj)
	{

		//TODO: Proper Implementation @TEMP

		//UBER TURBO TEST STUFF HERE

		auto core = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); ///Get Current Instance of graphics core

		if (sobj.custom_shader.empty())
		{
			std::string fragSrc;
			std::string vertSrc;

			std::shared_ptr<Shader> frag;
			std::shared_ptr<Shader> vert;
			ShaderGroup* sg;
			ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.frag", &fragSrc); ///Get Shader
			core->shaderFactory->genShader(fragSrc, ge::ShaderType::Shader_Fragment, &frag); ///Generate the shader

			ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.vert", &vertSrc); ///Get Shader
			core->shaderFactory->genShader(vertSrc, ge::ShaderType::Shader_Vertex, &vert); ///Generate the shader

			core->shaderFactory->genShaderGroup({ vert,frag }, &sg);
			//ALSO MEMORY LEAKS BECAUSE WHY NOT. AS STATED ABOVE UBER TURBO TEST STUFF

			shaders = sg;
			mesh->setShaderGroup(sg);

			mesh->registerUniform(DBL_STRINGIFY(ALBEDO_REF));
			mesh->registerUniform(DBL_STRINGIFY(SPECULAR_REF));
			// mesh->registerUniform("testLight.dir");
			// mesh->registerUniform("testLight.colour");

			// u_albedo = mesh->getUniform(DBL_STRINGIFY(ALBEDO_REF));
			// u_specular = mesh->getUniform(DBL_STRINGIFY(SPECULAR_REF));
			// u_testlight_col = mesh->getUniform("testLight.colour");
			// u_testlight_dir = mesh->getUniform("testLight.dir");

			mesh->registerTexture(albedo, 0);
			mesh->registerTexture(specular.get(), 1);
		}
		else
		{
			isDefault = false;
			ge::Scene s = GlobalMemory::get("ge_current_scene").getData<Scene>();
			
			shaders = s.shaderGroups.at(sobj.custom_shader).get();
			mesh->setShaderGroup(shaders);
		}
	


		mesh->registerUniform(DBL_STRINGIFY(CAMERA_POS));
		mesh->registerUniform(DBL_STRINGIFY(MVP_M));
		mesh->registerUniform(DBL_STRINGIFY(MVP_VP));
		mesh->registerUniform(DBL_STRINGIFY(TIME));


		u_camPos	= mesh->getUniform(DBL_STRINGIFY(CAMERA_POS));
		u_model		= mesh->getUniform(DBL_STRINGIFY(MVP_M));
		u_vp		= mesh->getUniform(DBL_STRINGIFY(MVP_VP));
		u_time		= mesh->getUniform(DBL_STRINGIFY(TIME));
		
		
		//Temporary Testing


	}

	void StaticObject::initTextures(Empty::StaticObject* sto)
	{
		if(sto->override_tex)
		{
			//for(auto tex: sto->)
		}
	}

	//TEST
	void StaticObject::initTest()
	{

	}



    Entity * StandardEntGen::genStaticObject(Empty::StaticObject so)
	{
		Entity* ent = new Entity();

		ent->name = "stat_obj";

        TransformComponent *tc     = new TransformComponent(ent);
		MeshRendererComponent *mrc = new MeshRendererComponent(ent,
                                                  GraphicsCore::ctx->meshFactory->newTriangleMesh(
                                                  *Scene::currentScene->meshes.find(so.mesh)->second.get()));

        
        //TODO: put this code into the Triangle Mesh Factory
		if(Scene::currentScene->shaderGroups.count(so.custom_shader)==0) //NOTE: TEMP  TODO: PUT THIS CHECK IN THE SCENE LOADER
			Log::critErr("StaticObject","Couldn't find shader group '"+so.custom_shader+"'");

        mrc->mesh->setShaderGroup(Scene::currentScene->shaderGroups.find(so.custom_shader)->second.get()); //TODO: make custom_shader just shader
        mrc->mesh->rebuffer();
        
        //MESH
        Scene::currentScene->textures.at(so.albedo).get()->setFiltering(TextureFilterType::Anisotropic_16x);
        Scene::currentScene->textures.at(so.specular).get()->setFiltering(TextureFilterType::Anisotropic_16x);

        mrc->mesh->registerTexture(Scene::currentScene->textures.at(so.albedo).get(), ALBEDO_LOC);
        mrc->mesh->registerTexture(Scene::currentScene->textures.at(so.specular).get(), SPECULAR_LOC);
        
        if(mrc->mesh->containsUniform(DBL_STRINGIFY(SKYBOX)))
		{

			mrc->mesh->registerCubeMap(Scene::currentScene->cubemaps[Scene::currentScene->skyboxCubemapName].get(), 0);
			mrc->mesh->setUniform(DBL_STRINGIFY(SKYBOX),Uniform::UniformContent(0));

			Scene::currentScene->cubemaps[Scene::currentScene->skyboxCubemapName].get()->setFiltering(TextureFilterType::Anisotropic_16x);

		}
		mrc->mesh->setUniform(DBL_STRINGIFY(ALBEDO_REF), Uniform::UniformContent(ALBEDO_LOC)); 
		mrc->mesh->setUniform(DBL_STRINGIFY(SPECULAR_REF), Uniform::UniformContent(SPECULAR_LOC)); 

        //TRANSFORM
        tc->setPosition(so.model.pos);
        tc->setScale(so.model.scale);
        tc->setRotation(so.model.rot);
        
        //NOTE: not good
        EntityManager::registerEntity(ent); //Maybe don't do, but we are still doing a component init.
        ent->insertComponent(tc);
        ent->insertComponent(mrc);
        
        Log::dbg("StaticObject", "TEST: "+std::to_string(so.model.pos.y)+", "+so.mesh+", "+Scene::currentScene->textures.at(so.albedo).get()->url);
        
        tc->insertToDefaultBatch();
        mrc->insertToDefaultBatch();
        
		return ent;
	}

}
