#include <engine/defaults/StaticObject.h>
#include <thread>
#include "memory/GlobalMemory.h"
#include "engine/scene/Scene.h"
#include "runtime/Runtime.h"
#include "console/ConsoleIO.h"

namespace ge
{
	StaticObject::StaticObject(Empty::StaticObject)
	{
		//get current scene
	}

	StaticObject::StaticObject(Empty::StaticObject eso, ge::Scene s)
	{
		//Check if on Render Thread.

		if(std::this_thread::get_id()!=GlobalMemory::get("ge_render_context_runtime").getData<Runtime>().getThreadID())
		{
			ConsoleIO::print("Static Object attempted initialization on non-render context thread" , MessageType::Error);
		}


		model = eso.model;

		albedo		= eso.albedo	== "" ? nullptr : s.textures.find(eso.albedo)	->second;
		specular	= eso.specular	== "" ? nullptr : s.textures.find(eso.specular)	->second;
		normal		= eso.normal	== "" ? nullptr : s.textures.find(eso.normal)	->second;
		roughness	= eso.roughness == "" ? nullptr : s.textures.find(eso.roughness)->second;



	}
}
