#pragma once
#pragma once
#include <engine/global_settings.pre>
#include "runtime/Runtime.h"
#include "runtime/GlobalRuntime.h"
#include <memory/GlobalMemory.h>
#include <graphics/GraphicsCore.h>
#include "graphics/empty_types/Scene.h"
#include "engine/scene/Scene.h"

struct LevelHandler
{
	LevelHandler();

	void update();
	void render();

	ge_START_CYCLE_HANDLER(LevelHandler)
		ge_GENERATE_TRAMPOLINE(update, UPDATE)
		ge_GENERATE_TRAMPOLINE(render, RENDER)
	ge_END_CYCLE_HANDLER



private:
	static ge::GraphicsCore* gc;

	static void loadLevel(void *);

	ge::Empty::Scene tempScene;
	ge::Scene		*currentScene;

	bool shouldLoadNext = true;
	bool shouldInitNew = false;

	int currentLevel = 0;

};
