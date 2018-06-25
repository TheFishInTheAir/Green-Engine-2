/*
//
// Created by Ethan Breit on 2017-08-15.
//

/*
 *
 *
 *
 * BROKEN
 *
 *
 *
 #1#


#pragma once

#include <runtime/Runtime.h>
#include <glm/glm.hpp>
#include <graphics/Camera.h>
#include <graphics/meshes/TriangleMesh.h>
#include "../GroupIDs.h"

using namespace ge;

struct ExampleModel
{


    Camera *camera;

    glm::mat4 model = glm::mat4(1);
    TriangleMesh *mesh;
    Uniform* u;

    void render();

    void update();

    ge_START_CYCLE_HANDLER(ExampleModel)
    ge_GENERATE_TRAMPOLINE(render,RENDER)
    ge_GENERATE_TRAMPOLINE(update,UPDATE)
    ge_END_CYCLE_HANDLER


    ExampleModel(std::string, Camera*);
};

*/
