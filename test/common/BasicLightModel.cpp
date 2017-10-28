//
// Created by Ethan Breit on 2017-08-13.
//

#include <common/BasicLightModel.h>
#include "loader/LoadMesh.h"

static GraphicsCore* core;

static Shader* vert;
static Shader* frag;
static ShaderGroup *sg;

static std::string fragSrc;
static std::string vertSrc;


void BasicLightModel::render()
{

	if(!isLoaded&&shouldLoad)
	{

		ConsoleIO::print("\n\n\n\nloading...\n\n\n\n\n\n");


		isLoaded = true;
		mesh = core->meshFactory->newTriangleMesh(m);
		core->textureFactory->genTexture(*img, &tex);
		tex->setFiltering(TextureFilterType::Anisotropic_16x);
		mesh->setShaderGroup(sg);

		mesh->registerUniform("mvp");
		mesh->registerUniform("testLight.dir");
		u = mesh->getUniform("mvp");
        lightDir = mesh->getUniform("testLight.dir");

		mesh->registerTexture(tex, 1);

		mesh->rebuffer();

		ConsoleIO::print("\n\n\n\nloaded.\n\n\n\n\n\n");


	}
	if (isLoaded)
	{
		u->setData(camera->vp*model);
        lightDir->setData(dirLight->dir);
		mesh->render();
	}
	//ConsoleIO::print("RENDERING \n");
}

void BasicLightModel::update()
{

	//ConsoleIO::print("UPDATING \n");


    if(shouldRotate)
    {
        if(rotationState)
        {
            rotation += 1.0f;
            model = glm::rotate(model,glm::radians(1.0f), {0.0f,1.0f,0.0f});
            if(rotation>360.0f)
                rotationState=false;
        }
        else
        {
            rotation -= 1.0f;
            model = glm::rotate(model,glm::radians(1.0f), {0.0f,-1.0f,0.0f});
            if(rotation<1.0f)
                rotationState=true;
        }
    }

    if(shouldHover)
    {
        if(positionState)
        {
            position += 0.01;
            model = glm::translate(model, {0.0f,0.01f,0.0f});
            if(position>1)
                positionState=false;
        }
        else
        {
            position -= 0.01;
            model = glm::translate(model, {0.0f,-0.01f,0.0f});
			if (position < -1) {
				positionState = true;
			}
        }
    }
}
static bool isInitialised	= false;
BasicLightModel::BasicLightModel(bool shouldRotate, bool shouldHover, Camera *c, std::string p, Image *i, LightDirectional* dirLight)
{
	ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

    ///Static setup
    static bool shouldSetup = true;
    if(shouldSetup)
    {
        shouldSetup = false;
		GlobalMemory::get("ge_render_context_runtime").getRawData<Runtime>()->enqueFunctionStatic(setup);
    }
	while (!isInitialised);
    ///BasicLightModel Instance Setup
    this->shouldRotate = shouldRotate;
    this->shouldHover  = shouldHover;
	this->p = p;
	this->img = i;
    this->dirLight = dirLight;

    camera = c;

	GlobalMemory::get("ge_loading_context_runtime").getRawData<Runtime>()->enqueFunction({ load, this });


}

void BasicLightModel::load(void* v)
{
	BasicLightModel* tm = reinterpret_cast<BasicLightModel*>(v);

	MeshLoader::loadTriangleMesh(tm->p, &tm->m);

	tm->shouldLoad = true;

}


void BasicLightModel::setup()
{
    /**
     *
     * One Time Mesh Setup (we only need one mesh because we are only changing uniforms)
     *
     */

    core = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); ///Get Current Instance of graphics core

    ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.frag", &fragSrc); ///Get Shader
    core->shaderFactory->genShader(fragSrc,ge::ShaderType::Shader_Fragment,&frag); ///Generate the shader

    ge::ResourceUtil::getRawStrResource("../res/shaders/basic/Textured/Textured.vert", &vertSrc); ///Get Shader
    core->shaderFactory->genShader(vertSrc,ge::ShaderType::Shader_Vertex,&vert); ///Generate the shader

	core->shaderFactory->genShaderGroup({ vert,frag }, &sg);

	isInitialised = true;
}

void BasicLightModel::cleanup()
{
    delete frag;    ///Delete Fragment Shader
    delete vert;    ///Delete Vertex Shader
}
