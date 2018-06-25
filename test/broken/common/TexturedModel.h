//
// Created by Ethan Breit on 2017-08-13.
//
/*
#pragma once


#include <runtime/Runtime.h>
#include <glm/glm.hpp>
#include <graphics/Camera.h>
#include <graphics/types/Shader.h>
#include <graphics/types/Uniform.h>
#include <graphics/GraphicsCore.h>
#include "GroupIDs.h"
#include <util/ResourceUtil.h>
#include <memory/GlobalMemory.h>

using namespace ge;

struct TexturedModel
{


    Camera *camera;

    glm::mat4 model = glm::mat4();


	Uniform* u;
	TriangleMesh *mesh;
	Image* img;
	Texture* tex;
	Empty::MeshData m;

    bool    shouldRotate;
    bool    shouldHover;
	bool	isLoaded	= false;
	bool	shouldLoad	= false;

    float   rotation = 0;
    float   position = 0.0f;

    bool    rotationState = false;
    bool    positionState = false;


	std::string p;

	TexturedModel(bool shouldRotate, bool shouldHover, Camera* c, std::string p, Image* T);
    static void setup();
    static void cleanup();
	static void load(void *);


    void render();

    void update();

    ge_START_CYCLE_HANDLER(TexturedModel)
        ge_GENERATE_TRAMPOLINE(render,RENDER)
        ge_GENERATE_TRAMPOLINE(update,UPDATE)
    ge_END_CYCLE_HANDLER


};


*/