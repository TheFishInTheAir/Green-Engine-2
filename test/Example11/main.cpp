
#include "../ExampleController.h"

#ifdef Enable_Example11

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


    //NOTE: HACK

        
    ge::Entity* ent = new ge::Entity();
    ent->name = "test_light";

    ge::LightComponent* testCmp = new ge::LightComponent(ent);
    ge::TransformComponent* testTCmp        = new ge::TransformComponent(ent);

    testCmp->light.colour = glm::vec3(1);
    testTCmp->setRotation(glm::quat(0.444,0.7777,0.444,0));

    testCmp->insertToDefaultBatch();
    testTCmp->insertToDefaultBatch();

    ent->insertComponent(testCmp);
    ent->insertComponent(testTCmp);

    ge::EntityManager::registerEntity(ent);



    ge::Entity* ent2 = new ge::Entity();
    ent2->name = "test_light2";

    ge::LightComponent* testCmp2            = new ge::LightComponent(ent2);
    ge::TransformComponent* testTCmp2        = new ge::TransformComponent(ent2);

    testCmp2->light.colour = glm::vec3(1,0,1);
    testTCmp2->setRotation(glm::quat(0.444,0.7777,-0.444,0));

    testCmp2->insertToDefaultBatch();
    testTCmp2->insertToDefaultBatch();

    ent2->insertComponent(testCmp2);
    ent2->insertComponent(testTCmp2);

    ge::EntityManager::registerEntity(ent2);

    ge::Entity* ent3 = new ge::Entity();
    ent3->name = "hs_cmp_test";

    ge::TransformComponent* testTCmp3        = new ge::TransformComponent(ent3);
    ge::HLMeshComponent*        testHL           = new ge::HLMeshComponent(ent3);
    testTCmp3->setPosition({4,1,0});

    testHL->setMeshData("demo/meshes/VSK_S.obj");
    testHL->setMaterial("OnyxTile");

    
    testTCmp3->insertToDefaultBatch();
    testHL->insertToDefaultBatch();

    ent3->insertComponent(testTCmp3);
    ent3->insertComponent(testHL);

    ge::EntityManager::registerEntity(ent3);
    
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
    
 
    
    //Light Test:

    
    //Camera Test:
    
    ge::Entity* camEnt = new ge::Entity();
    camEnt->name = "main_cam";

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
    ge::SceneLoader::loadSceneJson("scenes/g22_t4.json", emptyScene, true);
    
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
