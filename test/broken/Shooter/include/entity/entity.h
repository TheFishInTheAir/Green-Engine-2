#pragma once
#include "runtime/Runtime.h"
#include "engine/global_settings.pre"

struct Entity
{
	virtual void render() = 0;
	virtual void update() = 0;

	ge_START_CYCLE_HANDLER(Entity)
		ge_GENERATE_TRAMPOLINE(render, RENDER)
		ge_GENERATE_TRAMPOLINE(update, UPDATE)
	ge_END_CYCLE_HANDLER
};
