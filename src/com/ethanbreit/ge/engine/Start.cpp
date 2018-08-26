#include <ge/engine/Start.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/components/EngineControllerComponent.h>
#include <ge/graphics/pipeline/Pipeline.h>
namespace ge
{
	void simpleStart(SimpleEngineCreationInfo seci)
	{
		GraphicsCore* gc = new GraphicsCore(seci.g_api);
		gc->window->init(seci.wci);
        //gc->window->setClearColour(seci.clear_colour);
		for(auto r : seci.runtimes)
		{
			RuntimeManager::registerRuntime(r);
			r->start();
		}

		for(auto rg : seci.runtime_groups)
		{
			RuntimeManager::getRuntime(rg.second)->insertGroup(rg.first);
		}


		PipelineRuntimeRouter::ctx->init();
		if(seci.addInitPipeline)
		{
			GraphicsCore::ctx->currentPipeline = new Pipeline();
			PipelineStage* sDef = new PipelineStage();
			sDef->type = PipelineDrawType::Default;
			GraphicsCore::ctx->currentPipeline->stages.push_back(sDef);
		}

		
		EntityTag* no_export_tag = EntityManager::getOrCreateTag("no_export");

        //NOT VERY GOOD shouldn't really be an entity...
        Entity *engineCore = new Entity();
        EngineControllerComponent *ecc = new EngineControllerComponent(engineCore);
		
		engineCore->name = "engine_core";
		engineCore->registerToTag(no_export_tag);
		engineCore->insertComponent(ecc);

        EntityManager::registerEntity(engineCore);
        ecc->insertToDefaultBatch();
	}
}
