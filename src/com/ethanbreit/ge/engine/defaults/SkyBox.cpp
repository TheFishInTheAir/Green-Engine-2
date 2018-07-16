#include <ge/engine/defaults/SkyBox.h>
#include <ge/engine/scene/Scene.h>
//#include "common/BasicLightModel.h"
#include <ge/default_geom/Cube.h>
#include <ge/loader/LoadImage.h>
#include <array>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/ResourceUtil.h>
#include <ge/loader/LoadShader.h>

namespace ge
{
	GraphicsCore* SkyBox::_gCore;

	SkyBox::SkyBox(CubeMap* cm)
	{
		ge_REGISTER_RUNTIME_HANDLER;

		_gCore = GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getRawData<GraphicsCore>();

		ge::Scene s = GlobalMemory::get("ge_current_scene").getData<Scene>();

		if (std::this_thread::get_id() != GlobalMemory::get("ge_render_context_runtime").getData<Runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread", MessageType::Error);
		}

		currentCamera = GlobalMemory::get(DBL_STRINGIFY(CURRENT_CAMERA)).getRawData<Camera>();

		mesh = _gCore->meshFactory->newTriangleMesh(ge::dgeo::cube::getMeshData()); //TODO: fix to be shared pointer

		cubeMap = cm;

		cubeMap->setFiltering(TextureFilterType::Anisotropic_16x);

		initRenderer();
		//initTest();


		mesh->rebuffer();

		//Register to trampoline

		GlobalMemory::get("ge_render_runtime_group").getRawData<RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);

	}

	ge::SkyBox::SkyBox()
	{

		/*GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

		_gCore = GlobalMemory::get("ge_renderer_instance").getRawData<ge/graphicsCore>(); //TODO: have macro fir render instance. @MACRONAME

		ge::Scene s = GlobalMemory::get("ge_current_scene").getData<Scene>();

		if (std::this_thread::get_id() != GlobalMemory::get("ge_render_context_runtime").getData<ge/runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread", MessageType::Error);
		}

		currentCamera = GlobalMemory::get(DBL_STRINGIFY(CURRENT_CAMERA)).getRawData<Camera>();

		mesh = _gCore->meshFactory->newTriangleMesh(ge::dgeo::cube::getMeshData()); //TODO: fix to be shared pointer

		ge::Image* px;
		ge::Image* nx;
		
		ge::Image* py;
		ge::Image* ny;

		ge::Image* pz;
		ge::Image* nz;


		ImageLoader::loadImage("test_skybox/posx.jpg", &px);
		ImageLoader::loadImage("test_skybox/negx.jpg", &nx);

		ImageLoader::loadImage("test_skybox/posy.jpg", &py);
		ImageLoader::loadImage("test_skybox/negy.jpg", &ny);

		ImageLoader::loadImage("test_skybox/posz.jpg", &pz);
		ImageLoader::loadImage("test_skybox/negz.jpg", &nz);


		_gCore->textureFactory->genCubeMap({*px,*nx, *py,*ny, *pz,*nz}, &cubeMap);

		cubeMap->setFiltering(TextureFilterType::Anisotropic_16x);

		initRenderer();
		//initTest();


		mesh->rebuffer();

		//Register to trampoline

		GlobalMemory::get("ge_render_runtime_group").getRawData<ge/runtimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);

		*/ //TODO: @DEPRECATED
	}

	SkyBox::~SkyBox()
	{
		//delete u_vp;
		GlobalMemory::get("ge_render_runtime_group").getRawData<RuntimeGroup>()->ge_RUNTIME_GROUP_REMOVE_HEAP(this);
	}

	void SkyBox::render()
	{
		//TEMP GL
		glDepthFunc(GL_LEQUAL);

		/*u_vp->setData(currentCamera->proj * glm::mat4(glm::mat3(currentCamera->view)));
 		u_cubemap->setData(0);*/

		mesh->render();

		//TEMP GL
		glDepthFunc(GL_LESS);

	}

	void SkyBox::initRenderer()
	{

		/*std::string fragSrc;
		std::string vertSrc;

		Shader* frag;
		Shader* vert;
		ShaderGroup* sg;
		ge::ResourceUtil::getRawStrResource("engine/defaults/skybox/skybox.frag", &fragSrc); ///Get Shader
		_gCore->shaderFactory->genShader(fragSrc, ge::ShaderType::Shader_Fragment, &frag); ///Generate the shader

		ge::ResourceUtil::getRawStrResource("engine/defaults/skybox/skybox.vert", &vertSrc); ///Get Shader
		_gCore->shaderFactory->genShader(vertSrc, ge::ShaderType::Shader_Vertex, &vert); ///Generate the shader

		_gCore->shaderFactory->genShaderGroup({ vert,frag }, &sg);
		//ALSO MEMORY LEAKS BECAUSE WHY NOT. AS STATED ABOVE UBER TURBO TEST STUFF
		*/
		
		ge::ShaderLoader::loadShader("engine/defaults/skybox/skybox.gesm", &shaders);

		//shaders = sg;
		mesh->setShaderGroup(shaders);

		mesh->registerCubeMap(cubeMap, 0);



		mesh->registerUniform(DBL_STRINGIFY(MVP_VP));

		u_vp = mesh->getUniform(DBL_STRINGIFY(MVP_VP));


		//Temporary Testing

		mesh->registerUniform(DBL_STRINGIFY(CUBEMAP_0));

		u_cubemap = mesh->getUniform(DBL_STRINGIFY(CUBEMAP_0));

	}

	void setSkyBox(std::string s)
	{
		//TODO: FINISH
		ge::Scene sc = GlobalMemory::get("ge_current_scene").getData<Scene>();

		ConsoleIO::print("Setting SkyBox to " + s + "\n");

		if (!sc.cubemaps.count(s))
		{
			ConsoleIO::print("Incorrect SkyBox Name", MessageType::Error);
		}
		if (GlobalMemory::exists(MSTR(CURRENT_SKYBOX)))
		{
			SkyBox* sb = GlobalMemory::get(MSTR(CURRENT_SKYBOX)).getRawData<SkyBox>();
			delete sb;
			GlobalMemory::insert(MSTR(CURRENT_SKYBOX), { new SkyBox(sc.cubemaps.find(s)->second.get()),ReadableMemType::OTHER });
		}
		else
		{
			GlobalMemory::insert(MSTR(CURRENT_SKYBOX), { new SkyBox(sc.cubemaps.find(s)->second.get()),ReadableMemType::OTHER });
		}

	}

	void _makeSkyboxCity()
	{
		ConsoleIO::print("running thing pt 2\n");

		setSkyBox("city");
	}


	void makeSkyboxCity()
	{
		ConsoleIO::print("running thing\n");
		GlobalMemory::get("ge_render_context_runtime").getRawData<Runtime>()->enqueFunctionStatic(_makeSkyboxCity);
	}



}
