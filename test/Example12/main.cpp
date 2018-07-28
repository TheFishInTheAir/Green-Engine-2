
#include "../ExampleController.h"

#ifdef Enable_Example12

#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/engine/Start.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/components/LightComponent.h>
#include <ge/entity/component/components/CameraComponent.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/components/FreeCamControllerComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>
#include <ge/entity/component/components/HLMeshComponent.h>

#include <ge/entity/component/ComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/loader/LoadScene.h>
#include <ge/engine/scene/Scene.h>
#include <engine/global_settings.pre>
#include <ge/entity/component/ComponentManager.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/debug/DebugGUI.h>
#include <ge/util/ResourceUtil.h>
#include <ge/engine/scene/EntityDeserializer.h>

#include <json/json.hpp>

ge::GraphicsCore *gc;

bool uninitialised = true;

void runtimeInitHook()
{
    uninitialised = false;
}

void sceneInit(void* d)
{
    std::pair<ge::Scene*, ge::Empty::Scene*>* data = (std::pair<ge::Scene*, ge::Empty::Scene*>*) d;
    
    data->first->loadScene(*(data->second));

    ge::Debug::DebugGUI::enable();

    {
        std::string _file;
		ge::ResourceUtil::getRawStrResource("ge/t1.gesc", &_file);

        nlohmann::json tempJ = nlohmann::json::parse(_file);
        ge::Log::dbg("test1_");
        auto eList = ge::EntityDeserializer::deserializeJson(tempJ["entities"]);
        ge::Log::dbg("test2_");
        ge::EntityDeserializer::activateEntities(eList);
        ge::Log::dbg("test3_");
    }
};


int main()
{
    /**
     * New Fancy Engine Simple Start
     */
    
    ge::RuntimeGroup* update = new ge::RuntimeGroup();
    ge::RuntimeGroup* render = new ge::RuntimeGroup();
    ge::RuntimeGroup* postRender = new ge::RuntimeGroup();

    update->runtimeId = UPDATE;
    render->runtimeId = RENDER;
    postRender->runtimeId = POST_RENDER;

    
    
    ge::SimpleEngineCreationInfo seci = ge::SimpleEngineCreationInfo();
    
    seci.g_api = ge::GraphicsApi::OpenGL;
    
    seci.wci.height = 1080;
    seci.wci.width = 1920;
    seci.wci.gl_major_version = 3;
    seci.wci.gl_minor_version = 3;
    seci.wci.clear_colour = glm::vec3(79.0f/255, 100.0f/255, 144.0f/255);
    //seci.wci.has_depth_buffer = false; //NOTE: TEST
    seci.runtimes.push_back(new ge::Runtime(RUNTIME_MAIN));
    seci.runtime_groups.push_back({ update, RUNTIME_MAIN });
    seci.runtime_groups.push_back({ render, RUNTIME_MAIN });
    seci.runtime_groups.push_back({ postRender, RUNTIME_MAIN });

    ge::simpleStart(seci);
    
    
    gc = ge::GraphicsCore::ctx;
    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunctionStatic(runtimeInitHook);
    
    while(uninitialised); //wait until Runtimes are created
    
 
    
    //Camera Test:
    
    ge::Entity* camEnt = new ge::Entity();
    camEnt->name = "main_cam";

    ge::FreeCamControllerComponent* fcc = new ge::FreeCamControllerComponent(camEnt);
    ge::CameraComponent* cmpComponent   = new ge::CameraComponent(camEnt);
    ge::TransformComponent* tCmp        = new ge::TransformComponent(camEnt);
    
    camEnt->insertComponent(tCmp);
    camEnt->insertComponent(cmpComponent);
    camEnt->insertComponent(fcc);
    
    tCmp->insertToDefaultBatch();
    cmpComponent->insertToDefaultBatch();
    fcc->insertToDefaultBatch();
    
    cmpComponent->makeDisplay();
    
    ge::EntityManager::registerEntity(camEnt);

    
    //Scene Test
    ge::Empty::Scene* emptyScene = new ge::Empty::Scene();
    ge::SceneLoader::loadSceneJson("ge/t1.gesc", emptyScene, true);


    
    ge::Scene *scene = new ge::Scene();
    ge::Scene::currentScene = scene;
    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunction({sceneInit, new std::pair<ge::Scene*, ge::Empty::Scene*>(scene, emptyScene)});



    while(!gc->window->shouldClose()) // 79 148 248
        gc->window->poll();
    
    //TODO: cleanup

}

#endif
