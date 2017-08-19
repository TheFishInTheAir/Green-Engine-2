//
// Created by Ethan Breit on 2017-08-13.
//

#pragma once


#include <runtime/Runtime.h>
#include <glm/glm.hpp>
#include <graphics/Camera.h>
#include <graphics/types/Shader.h>
#include <graphics/types/Uniform.h>
#include <graphics/GraphicsCore.h>
#include "GroupIDs.h"


using namespace ge;

struct Triangle
{


    Camera *camera;

    glm::mat4 model = glm::mat4(1);





    bool    shouldRotate;
    bool    shouldHover;

    float   rotation = 0;
    float   position = 0.0f;

    bool    rotationState = false;
    bool    positionState = false;


    static void setup();
    static void cleanup();


    void render();

    void update();

    ge_START_CYCLE_HANDLER(Triangle)
        ge_GENERATE_TRAMPOLINE(render,RENDER)
        ge_GENERATE_TRAMPOLINE(update,UPDATE)
    ge_END_CYCLE_HANDLER


    Triangle(bool, bool, Camera*);
};


