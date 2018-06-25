#pragma once
#include <engine/global_settings.pre>
#include "runtime/Runtime.h"
#include "graphics/Camera.h"
#include "graphics/GraphicsCore.h"
#include "debug/debug_entities/ColouredBox.h"


struct Player
{
	void update();
	void render();

	Player();
	ge_START_CYCLE_HANDLER(Player)
		ge_GENERATE_TRAMPOLINE(update, UPDATE)
		ge_GENERATE_TRAMPOLINE(render, RENDER)
	ge_END_CYCLE_HANDLER

private:
	ge::Camera* cam;

	ge::Debug::DebugColouredBox* tempModel;

	float x = 0, y = 0;
	float vX = 0, vY = 0;
	glm::vec2 dir;

	static ge::GraphicsCore* gc;

};
