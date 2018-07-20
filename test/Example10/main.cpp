
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
#include <ge/entity/component/components/CameraComponent.h>
#include <ge/entity/component/components/TransformComponent.h>
#include <ge/entity/component/components/FreeCamControllerComponent.h>
#include <ge/entity/component/batches/DefaultComponentBatch.h>

#include <ge/entity/component/ComponentBatch.h>
#include <ge/console/Log.h>
#include <ge/loader/LoadScene.h>
#include <ge/engine/scene/Scene.h>
#include <engine/global_settings.pre>
#include <ge/entity/component/ComponentManager.h>
#include <ge/input/KeyboardHandler.h>
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
};

struct TestERComponent : public ge::Component
{
    TestERComponent(ge::Entity* e) : ge::Component(e){};
    ge::TransformComponent* tcomp;

    void defaultInit()
    {}
    void insertToDefaultBatch()
    {
        if(!ge::ComponentManager::containsComponentBatch("DefaultComponentBatch", getTypeName()))
        {
            ge::DefaultComponentBatch* cmp = new ge::DefaultComponentBatch();
            cmp->setComponentType(getTypeName());
            
            ge::ComponentManager::registerComponentBatch(cmp);
        }
        
        ge::ComponentManager::getComponentBatch("DefaultComponentBatch", getTypeName())->softInsert(this);
        tcomp = (ge::TransformComponent*)ent->components["TransformComponent"];
    }
    float test = 0.005;
    void cycle()
    {
        if(ge::KeyboardHandler::keyDownSticky(ge::KeyboardKey::W))
            tcomp->setPosition(tcomp->getPosition()+glm::vec3(0, 0, -test));
        if(ge::KeyboardHandler::keyDownSticky(ge::KeyboardKey::A))
            tcomp->setPosition(tcomp->getPosition()+glm::vec3(-test, 0, 0));
        if(ge::KeyboardHandler::keyDownSticky(ge::KeyboardKey::S))
            tcomp->setPosition(tcomp->getPosition()+glm::vec3(0, 0, test));
        if(ge::KeyboardHandler::keyDownSticky(ge::KeyboardKey::D))
            tcomp->setPosition(tcomp->getPosition()+glm::vec3(test, 0, 0));

    }
    void destroy()
    {
        
    }
    std::string getTypeName()
    {
        return "TesterBoi";
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
    seci.wci.gl_major_version = 4;
    seci.wci.gl_minor_version = 1;
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
    
 
    
    //Entity Test:
    
    ge::Entity* ent = new ge::Entity();
    ge::TestComponent* testCmp = new ge::TestComponent(ent);
    
    testCmp->insertToDefaultBatch();
    
    ent->insertComponent(testCmp);
    
    ge::EntityManager::registerEntity(ent);
    
    //Camera Test:
    
    ge::Entity* camEnt = new ge::Entity();
    ge::FreeCamControllerComponent* fcc = new ge::FreeCamControllerComponent(camEnt);

    ge::CameraComponent* cmpComponent   = new ge::CameraComponent(camEnt);
    ge::TransformComponent* tCmp        = new ge::TransformComponent(camEnt);
    //TestERComponent *TESTBOI = new TestERComponent(camEnt);
    
    camEnt->insertComponent(tCmp);
    camEnt->insertComponent(cmpComponent);
    camEnt->insertComponent(fcc);
    //camEnt->insertComponent(TESTBOI);
    
    tCmp->insertToDefaultBatch();
    //TESTBOI->insertToDefaultBatch();
    cmpComponent->insertToDefaultBatch();
    fcc->insertToDefaultBatch();
    
    cmpComponent->makeDisplay();
    
    ge::EntityManager::registerEntity(camEnt);

    
    //Scene Test
    ge::Empty::Scene* emptyScene = new ge::Empty::Scene();
    ge::SceneLoader::loadSceneJson("scenes/g22_t1.json", emptyScene, true);
    
    ge::Scene *scene = new ge::Scene();
    ge::Scene::currentScene = scene;
    
    ge::RuntimeManager::getRuntime(RUNTIME_MAIN)->enqueFunction({sceneInit, new std::pair<ge::Scene*, ge::Empty::Scene*>(scene, emptyScene)});

    
 
    //batch->cycle();
    //testCmp->destroy();
    //batch->hardRemove(testCmp->batchId);
    
    ge::Log::dbg("UPDATE");
    while(!gc->window->shouldClose()) // 79 148 248
        gc->window->poll();
    
    //TODO: cleanup

}

#endif
