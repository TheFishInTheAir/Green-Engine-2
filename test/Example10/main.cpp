
#include "../ExampleController.h"
#include "GroupIDs.h"

#ifdef Enable_Example10

#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/engine/Start.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/components/TestComponent.h>
#include <ge/entity/component/ComponentBatch.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/console/Log.h>

#include <engine/global_settings.pre>


ge::GraphicsCore *gc;

bool uninitialised = true;

void runtimeInitHook()
{
    uninitialised = false;
}


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
    seci.wci.gl_minor_version = 1;
    seci.runtimes.push_back(new ge::Runtime(RUNTIME_MAIN));
    seci.runtime_groups.push_back({ update, RUNTIME_MAIN });
    seci.runtime_groups.push_back({ render, RUNTIME_MAIN });
    
    ge::simpleStart(seci);
    
    
    gc = ge::GraphicsCore::ctx;
    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunctionStatic(runtimeInitHook);
    
    while(uninitialised); //wait until Runtimes are created
    
    //Entity Test:
    
    ge::Entity* ent = new ge::Entity();
    ge::TestComponent* testCmp = new ge::TestComponent(ent);
    
    testCmp->insertToDefaultBatch();
    
    ent->insertComponent(testCmp);
    
    ge::EntityManager::registerEntity(ent);
    
    //batch->cycle();
    //testCmp->destroy();
    //batch->hardRemove(testCmp->batchId);
    
    
    while(!gc->window->shouldClose())
        gc->window->poll();
    
    //TODO: cleanup

}

#endif
