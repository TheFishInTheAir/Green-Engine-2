#include <ge/engine/Start.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/components/EngineControllerComponent.h>
namespace ge
{
	void simpleStart(SimpleEngineCreationInfo seci)
	{
		GraphicsCore* gc = new GraphicsCore(seci.g_api);
		gc->window->init(seci.wci);
        gc->window->setClearColour(seci.clear_colour);
		for(auto r : seci.runtimes)
		{
			RuntimeManager::registerRuntime(r);
			r->start();
		}

		for(auto rg : seci.runtime_groups)
		{
			RuntimeManager::getRuntime(rg.second)->insertGroup(rg.first);
		}
        
        //NOT VERY GOOD
        Entity *engineCore = new Entity();
        EngineControllerComponent *ecc = new EngineControllerComponent(engineCore);
        
        EntityManager::registerEntity(engineCore);
        ecc->insertToDefaultBatch();
	}
}
