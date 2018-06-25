//
// Created by Ethan Breit on 2017-07-28.
//


#include "../ExampleController.h"
#include "GroupIDs.h"
#ifdef Enable_Example9
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/engine/Start.h>

ge::GraphicsCore *gc;

int main()
{

	/**
	 * New Fancy Engine Simple Start
	 */

	ge::RuntimeGroup* update = new ge::RuntimeGroup();
	ge::RuntimeGroup* render = new ge::RuntimeGroup();
	update->runtimeId = UPDATE;
	render->runtimeId = RENDER;



	ge::SimpleEngineCreationInfo seci = ge::SimpleEngineCreationInfo();
	
	seci.g_api = ge::GraphicsApi::OpenGL;

	seci.wci.height = 1080;
	seci.wci.width = 1920;
	seci.wci.gl_major_version = 4;
	seci.wci.gl_minor_version = 4;
	seci.runtimes.push_back(new ge::Runtime("main"));
	seci.runtime_groups.push_back({ update, "main" });
	seci.runtime_groups.push_back({ render, "main" });
	ge::simpleStart(seci);

	gc = ge::GraphicsCore::ctx;


	while(!gc->window->shouldClose())
		gc->window->poll();
	
	//TODO: cleanup
}



#endif