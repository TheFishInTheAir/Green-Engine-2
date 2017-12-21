//
// Created by Ethan Breit on 2017-07-28.
//
#include "../ExampleController.h"
#include "common/TexturedModel.h"
#include "loader/LoadImage.h"
#include "loader/ShaderPreprocessor.h"
#include "common/BasicLightModel.h"
#include "graphics/types/lights/LightDirectional.h"
#include "debug/debug_entities/ColouredBox.h"
#ifdef Enable_Example8
#include <gl/glew.h>
#include <error/Error.h>
#include <runtime/Runtime.h>
#include <console/ConsoleIO.h>
#include <util/ResourceUtil.h>
#include <graphics/GraphicsCore.h>
#include <graphics/Camera.h>
#include <debug/FreeMove.h>
#include <common/Triangle.h>
#include <memory/GlobalMemory.h>
ge::GraphicsCore *gc;

bool initialised = false;

FreeMove fm;

Camera *camera;

Runtime *updateRun;

Runtime *renderRun;

Runtime *loadingRun;


LightDirectional* dirLight;


RuntimeGroup *updateGroup;

RuntimeGroup *renderGroup;

//RuntimeGroup *loadingGroup;


RuntimeGroup *postRenderGroup;

RuntimeGroup *preRenderGroup;

void startWindow()
{

	gc->window->makeCurrentThread(renderRun);
	gc->window->setClearColour({ 0.2f, 0.3f, 0.3f }); /// self explanatory (has to be here becuase OpenGL is bad with multithreading)

	initialised = true;
}

struct example
{

    void preRender()
    {
        gc->window->clear();
    }


    void postRender()
    {


        gc->window->swap(); /// swap buffers
    }

    void update()
    {
        fm.update(camera);
    	camera->update();

		//Light Rotation

		//dirLight->dir = glm::rotateY(dirLight->dir, 0.02f);

        if(gc->window->shouldClose())
        {
            renderRun->end(); ///End threads
            updateRun->end();
            gc->window->cleanup(); /// cleanup and terminate window
        }
    }

    ge_START_CYCLE_HANDLER(example)
            ge_GENERATE_TRAMPOLINE(preRender, PRE_RENDER)
            ge_GENERATE_TRAMPOLINE(postRender, POST_RENDER)
            ge_GENERATE_TRAMPOLINE(update, UPDATE)
    ge_END_CYCLE_HANDLER

    example()
    {
        ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

    }


};




int main()
{
    /**
     * Initialise Graphics
     */
    gc = new ge::GraphicsCore(ge::GraphicsApi::OpenGL);

    //Lighting Setup


	dirLight = new LightDirectional();
	dirLight->dir = glm::vec3(1, -0.5, 0);
	dirLight->colour = glm::vec3(1, 1, 1);

    /**
     *
     *
     * Runtime
     *
     *
     */

    updateRun     = new ge::Runtime("Update Runtime", 60);
    renderRun     = new ge::Runtime("Render Runtime", 60);
	loadingRun	  = new ge::Runtime("Loading Runtime", 1);

    updateGroup      = new ge::RuntimeGroup();
    renderGroup      = new ge::RuntimeGroup();
    preRenderGroup   = new ge::RuntimeGroup();
    postRenderGroup  = new ge::RuntimeGroup();
	//loadingGroup	 = new ge::RuntimeGroup();

    updateGroup->runtimeId       = UPDATE;
    renderGroup->runtimeId       = RENDER;
    preRenderGroup->runtimeId    = PRE_RENDER;
    postRenderGroup->runtimeId   = POST_RENDER;
	//loadingGroup->runtimeId		 = LOADER;

    updateRun->insertGroup(updateGroup);

    renderRun->insertGroup(preRenderGroup,0);
    renderRun->insertGroup(renderGroup,1);
    renderRun->insertGroup(postRenderGroup,2);

	//loadingRun->insertGroup(loadingGroup);

	GlobalMemory::insert("ge_loading_context_runtime", { loadingRun,ReadableMemType::OTHER });

	/**
	 *
	 * Window Init
	 *
	 */

	ge::WindowConstructorInfo wci = ge::WindowConstructorInfo();
	
	wci.height = 1080;
	wci.width  = 1920;
	wci.glMajorVersion = 2;
	wci.glMinorVersion = 2;
	ge_Error_ADDTRACE(
		GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>()->window->init(ge::WindowConstructorInfo()));

    renderRun->enqueFunctionStatic(startWindow);

	while (!initialised);
    /**
     * Generate and Configure Camera
     */

    camera = new ge::Camera();
    camera->nearCull = 0.1f;
    camera->farCull  = 100.0f;
    camera->aspectRatio = ((float)wci.width)/((float)wci.height);
    //camera.fov = 90.0f/100.0f; ///Divide by 100 to convert fov to radians.
    camera->fov   = glm::radians(45.0f);
    ///initialising data
    camera->up = glm::vec3(0,1,0);
    camera->pos = glm::vec3(0,0,0);
    camera->dir = glm::vec3(0,0,-1);



    /**
     * Generate and Configure Debug FreeMove TODO: Add Debug to namespace
     */

    fm = ge::FreeMove();
    fm.speed = 0.05f;
    fm.lookSpeed = 0.0007f;
    fm._position = glm::vec3(0);

    ///initialising data
    fm._horizontalAngle = 0;
    fm._verticalAngle = 0;



    /**
     *
     * generate and register new runtime items
     *
     */
	
    example *e = new example();
    updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(e);
    preRenderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(e);
    postRenderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(e);
	
	
	//
	// load scene
	//
	
	
	Triangle *t1l = new Triangle(false, false, camera);
	t1l->model = glm::translate(t1l->model, dirLight->dir);
	t1l->model = glm::scale(t1l ->model, glm::vec3(0.2,0.2,0.2));

	updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1l);
	renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1l);

	Debug::DebugColouredBox *box = new Debug::DebugColouredBox(camera, -(dirLight->dir)*8);
	box->model = glm::scale(box->model, { 0.5f,0.5f,0.5f });
	box->colour = {1,1,1};
	renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(box);


    Triangle *t1 = new Triangle(false, true, camera);
    t1->model = glm::translate(t1->model, {3.0f,0.0f,0.0f});

    updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1);
    renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1);
	
	Triangle *t2 = new Triangle(true, false, camera);

	t2->model = glm::translate(t2->model, { -3.0f,0.0f,0.0f });

	updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t2);
	renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t2);

	Image *im;
	ImageLoader::loadImage("VoodooSkull_BaseColor.png", &im);

	Image *im_spec;
	ImageLoader::loadImage("VoodooSkullRough.png", &im_spec);

	BasicLightModel *texm = new BasicLightModel(false, false /* enable this one to rotate */, camera, "VoodooSkull.obj", im, im_spec, dirLight);
	texm->model = glm::scale(texm->model, { 0.05f,0.05f,0.05f });
	texm->model = glm::rotate(texm->model, glm::radians(90.0f), glm::vec3(0.0f,1.0f,0.0f));

	updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(texm);
	renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(texm);
	/*
	std::string data;
	ResourceUtil::getRawStrResource("shaders/debug/preprocessor_test.glsl", &data);
	ConsoleIO::Print("\n" + data + "\n\n",MessageType::Debug);
	ShaderPreprocessor::process(data);
    */
	ConsoleIO::print("Dumping Global Memory Keys:\n\n", MessageType::Debug);

	std::vector<std::string> globalTest = GlobalMemory::getAllEntries();
	for(std::string s : globalTest)
	{
		ConsoleIO::print(s+"\n", MessageType::Debug);
	}
	ConsoleIO::print("\n");


    while(true)
        gc->window->poll(); /// poll window events;

}



#endif