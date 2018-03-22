#include <Shooter/include/levelhandler.h>
#include "loader/LoadScene.h"
#define M_PI 3.14159265358979323846
LevelHandler::LevelHandler()
{
	ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

	currentScene = new ge::Scene();
	currentScene->init();
	ge::GlobalMemory::insert("ge_current_scene", { currentScene, ge::ReadableMemType::OTHER });

	ge::GlobalMemory::get("ge_render_runtime_group").getRawData<ge::RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);
	ge::GlobalMemory::get("ge_update_runtime_group").getRawData<ge::RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);
	//Should Set Current Scene
}

void LevelHandler::update()
{
	if (shouldLoadNext)
	{
		shouldLoadNext = false;
		currentLevel++;
		ge::GlobalMemory::get("ge_load_context_runtime").getRawData<ge::Runtime>()->enqueFunction({loadLevel,this});


	}

	static ge::LightDirectional* light = ge::GlobalMemory::get("testlight").getRawData<ge::LightDirectional>();
	static float degree = 0.0f;
	light->dir.x = cos(degree) + sin(degree);
	light->dir.z = -sin(degree) + cos(degree);
	degree += 0.01f;
	if (degree > M_PI*2)
		degree = 0.0f;
}

void LevelHandler::render()
{
	if (shouldInitNew)
	{
		shouldInitNew = false;
		currentScene->loadScene(tempScene);
	}
}

void LevelHandler::loadLevel(void* tempT)
{
	LevelHandler* t = (LevelHandler*)tempT;
	switch (t->currentLevel)
	{
	case 1:

		ge::SceneLoader::loadSceneJson("demo/shooter/levels/l1.json", &t->tempScene, true);

		t->shouldInitNew = true;
		break;
	}
}
