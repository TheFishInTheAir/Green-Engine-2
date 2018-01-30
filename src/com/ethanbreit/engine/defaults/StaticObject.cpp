#include <engine/defaults/StaticObject.h>
#include <thread>
#include "memory/GlobalMemory.h"
#include "engine/scene/Scene.h"
#include "runtime/Runtime.h"
#include "console/ConsoleIO.h"
#include "util/ResourceUtil.h"
#include "graphics/GraphicsCore.h"
namespace ge
{
	GraphicsCore* StaticObject::_gCore;

	StaticObject::StaticObject(Empty::StaticObject eso)
	{
		//Init Trampoline

		GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

		_gCore = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); //TODO: have macro.
		
		//TODO: make function to do this


		//get current scene

		ge::Scene s = GlobalMemory::get("ge_current_scene").getData<Scene>();


		if (std::this_thread::get_id() != GlobalMemory::get("ge_render_context_runtime").getData<Runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread", MessageType::Error);
		}

		currentCamera = GlobalMemory::get(DBL_STRINGIFY(CURRENT_CAMERA)).getRawData<Camera>();

		testlight = GlobalMemory::get("testlight").getRawData<LightDirectional>();

		model = eso.model;
		mesh  = _gCore->meshFactory->newTriangleMesh(*s.meshes.find(eso.mesh)->second.get()); //todo: fix to be shared pointer
		
		
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


		initRenderer();
		initTest();


		mesh->rebuffer();

		//Register to trampoline

		GlobalMemory::get("ge_render_runtime_group").getRawData<RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this); //TODO: where I left off
		
	}
	void StaticObject::render()
	{

		u_vp->setData((currentCamera->vp));
		u_model->setData(model.matrix);
		u_testlight_dir->setData(testlight->dir);
		u_testlight_col->setData(testlight->colour);
		u_albedo->setData(0);
		u_specular->setData(1);
		u_camPos->setData(currentCamera->pos);

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


	void StaticObject::initRenderer()
	{
		//UBER TURBO TEST STUFF HERE

		auto core = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); ///Get Current Instance of graphics core

		std::string fragSrc;
		std::string vertSrc;

		Shader* frag;
		Shader* vert;
		ShaderGroup* sg;
		ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.frag", &fragSrc); ///Get Shader
		core->shaderFactory->genShader(fragSrc, ge::ShaderType::Shader_Fragment, &frag); ///Generate the shader

		ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.vert", &vertSrc); ///Get Shader
		core->shaderFactory->genShader(vertSrc, ge::ShaderType::Shader_Vertex, &vert); ///Generate the shader

		core->shaderFactory->genShaderGroup({ vert,frag }, &sg);
		//ALSO MEMORY LEAKS BECAUSE WHY NOT. AS STATED ABOVE UBER TURBO TEST STUFF
		shaders = sg;
		mesh->setShaderGroup(sg);
	


		mesh->registerUniform(DBL_STRINGIFY(CAMERA_POS));
		mesh->registerUniform(DBL_STRINGIFY(MVP_M));
		mesh->registerUniform(DBL_STRINGIFY(MVP_VP));

		u_camPos	= mesh->getUniform(DBL_STRINGIFY(CAMERA_POS));
		u_model		= mesh->getUniform(DBL_STRINGIFY(MVP_M));
		u_vp		= mesh->getUniform(DBL_STRINGIFY(MVP_VP));
		
		
		//Temporary Testing

		mesh->registerUniform(DBL_STRINGIFY(ALBEDO_REF));
		mesh->registerUniform(DBL_STRINGIFY(SPECULAR_REF));
		mesh->registerUniform("testLight.dir");
		mesh->registerUniform("testLight.colour");

		u_albedo	= mesh->getUniform(DBL_STRINGIFY(ALBEDO_REF));
		u_specular	= mesh->getUniform(DBL_STRINGIFY(SPECULAR_REF));
		u_testlight_col = mesh->getUniform("testLight.colour");
		u_testlight_dir = mesh->getUniform("testLight.dir");

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
		mesh->registerTexture(albedo, 0);
		mesh->registerTexture(specular.get(), 1);
	    

	}



}
