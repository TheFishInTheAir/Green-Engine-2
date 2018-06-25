#include <ge/engine/Start.h>
#include <ge/graphics/GraphicsCore.h>

namespace ge
{
	void simpleStart(SimpleEngineCreationInfo seci)
	{
		GraphicsCore* gc = new GraphicsCore(seci.g_api);
		gc->window->init(seci.wci);

		for(auto r : seci.runtimes)
		{
			RuntimeManager::registerRuntime(r);
			r->start();
		}

		for(auto rg : seci.runtime_groups)
		{
			RuntimeManager::getRuntime(rg.second)->insertGroup(rg.first);
		}
	}
}
