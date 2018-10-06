
#include "../ExampleController.h"

#ifdef Enable_Example13

#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/engine/Start.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/components/DirectionalLightComponent.h>
#include <ge/entity/component/components/CameraComponent.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/components/FreeCamControllerComponent.h>
#include <ge/entity/component/components/AudioListinerComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/components/HLMeshComponent.h>
#include <ge/entity/component/components/RuntimeMaterialComponent.h>

#include <ge/entity/component/ComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/loader/LoadScene.h>
#include <ge/engine/scene/Scene.h>
#include <engine/global_settings.pre>
#include <ge/entity/component/ComponentManager.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/debug/DebugGUI.h>
#include <ge/util/ResourceUtil.h>

#include <ge/audio/AudioSource.h>
#include <ge/audio/AudioClip.h>
#include <ge/audio/AudioController.h>

#include <ge/loader/LoadAudioClip.h>

#include <json/json.hpp>

ge::GraphicsCore *gc;
    ge::FrameBuffer* fb;

bool* uninitialised = nullptr;

void runtimeInitHook()
{
    *uninitialised = false;
}
const std::string MAP_LOC = "ge/t5.gesc";

void sceneInit(void* d)
{
    std::pair<ge::Scene*, ge::Empty::Scene*>* data = (std::pair<ge::Scene*, ge::Empty::Scene*>*) d;
    
    data->first->loadScene(*(data->second));

    ge::Debug::DebugGUI::enable();

};

void addThing()
{

	ge::Entity* camEnt = new ge::Entity();
	camEnt->name = "main_cam";

	ge::FreeCamControllerComponent* fcc = new ge::FreeCamControllerComponent(camEnt);
	ge::CameraComponent* cmpComponent = new ge::CameraComponent(camEnt);
	ge::TransformComponent* tCmp = new ge::TransformComponent(camEnt);
	ge::AudioListinerComponent* aCmp = new ge::AudioListinerComponent(camEnt);

	camEnt->insertComponent(tCmp);
	camEnt->insertComponent(cmpComponent);
	camEnt->insertComponent(fcc);
	camEnt->insertComponent(aCmp);

	tCmp->insertToDefaultBatch();
	cmpComponent->insertToDefaultBatch();
	fcc->insertToDefaultBatch();
	aCmp->insertToDefaultBatch();

	//cmpComponent->makeDisplay();

	ge::EntityManager::registerEntity(camEnt);
}


int main()
{
    /**
     * New Fancy Engine Simple Start
     */
	uninitialised = new bool();
	*uninitialised = true;
    ge::RuntimeGroup* update = new ge::RuntimeGroup();

    update->runtimeId = UPDATE;

    
    
    ge::SimpleEngineCreationInfo seci = ge::SimpleEngineCreationInfo();
    
    seci.g_api = ge::GraphicsApi::OpenGL;
    
    seci.wci.height = 1080;
    seci.wci.width = 1920;
    seci.wci.gl_major_version = 3;
    seci.wci.gl_minor_version = 3;
    seci.wci.clear_colour = glm::vec3(79.0f/255, 100.0f/255, 144.0f/255);
    seci.wci.samples = 0;
    //seci.wci.has_depth_buffer = false; //NOTE: TEST
    seci.runtimes.push_back(new ge::Runtime(RUNTIME_MAIN));
    seci.runtime_groups.push_back({ update, RUNTIME_MAIN });

    ge::simpleStart(seci);
	gc = ge::GraphicsCore::ctx;

	
	
	//addThing();
   

    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunctionStatic(runtimeInitHook);
    while(*uninitialised) ge::Log::dbg("FUCK"); //wait until Runtimes are created

    //Scene Test
    ge::Empty::Scene* emptyScene = new ge::Empty::Scene();
    ge::SceneLoader::loadSceneJson(MAP_LOC, emptyScene, true);


    
    ge::Scene *scene = new ge::Scene();
    ge::Scene::currentScene = scene;
    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunction({sceneInit, new std::pair<ge::Scene*, ge::Empty::Scene*>(scene, emptyScene)});

	

	gc->window->mainThreadPollLoop();
    //while(!gc->window->shouldClose()) // 79 148 248
    //     gc->window->poll();
    
    //TODO: cleanup

}

#endif
