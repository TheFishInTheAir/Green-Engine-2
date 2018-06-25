//
// Created by Ethan Breit on 2017-07-28.
//
//#include "../../ExampleController.h"

#ifdef Enable_Example5
#include <error/Error.h>
#include <runtime/Runtime.h>
#include <console/ConsoleIO.h>
#include <util/ResourceUtil.h>
#include <graphics/GraphicsCore.h>
#include <graphics/Camera.h>
#include <debug/FreeMove.h>
#include <common/Triangle.h>
#include <memory/GlobalMemory.h>
#include <common/ExampleModel.h>
#include <input/MouseHandler.h>

ge::GraphicsCore *gc;
void startWindow()
{
    gc->window->makeCurrentThread();
    gc->window->setClearColour({0.2f, 0.3f, 0.3f}); /// self explanatory (has to be here becuase OpenGL and GLFW is bad with multithreading)
    ge::MouseHandler::resetMousePos();

}

FreeMove fm;

Camera *camera;

Runtime *updateRun;

Runtime *renderRun;


RuntimeGroup *updateGroup;

RuntimeGroup *renderGroup;

RuntimeGroup *postRenderGroup;

RuntimeGroup *preRenderGroup;

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


    ConsoleIO::print(std::to_string(((unsigned char)0x1f&0xf0)>>4));


    /**
     * Initialise Graphics
     */
    gc = new ge::GraphicsCore(ge::GraphicsApi::OpenGL);

    ge_Error_ADDTRACE(
            GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>()->window->init(ge::WindowConstructorInfo()));

    /**
     *
     *
     * Runtime
     *
     *
     */

    updateRun     = new ge::Runtime("Update Runtime", 60);
    renderRun     = new ge::Runtime("Render Runtime", 60);

    updateGroup      = new ge::RuntimeGroup();
    renderGroup      = new ge::RuntimeGroup();
    preRenderGroup   = new ge::RuntimeGroup();
    postRenderGroup  = new ge::RuntimeGroup();

    updateGroup->runtimeId       = UPDATE;
    renderGroup->runtimeId       = RENDER;
    preRenderGroup->runtimeId    = PRE_RENDER;
    postRenderGroup->runtimeId   = POST_RENDER;

    updateRun->insertGroup(updateGroup);
    renderRun->insertGroup(preRenderGroup,0);
    renderRun->insertGroup(renderGroup,1);
    renderRun->insertGroup(postRenderGroup,2);


    renderRun->enqueFunction(startWindow);


    /**
     * Generate and Configure Camera
     */

    camera = new ge::Camera();
    camera->nearCull = 0.1f;
    camera->farCull  = 100.0f;
    camera->aspectRatio = 1.6f;
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
    fm.lookSpeed = 0.003f;
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


    ExampleModel *t1 = new ExampleModel("../res/models/debug/slime_vert.obj", camera);
    //t1->model = glm::translate(t1->model, {1.0f,0.0f,0.0f});

    updateGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1);
    renderGroup->ge_RUNTIME_GROUP_INSERT_HEAP(t1);


    while(true)
        gc->window->poll(); /// poll window events;

}



#endif