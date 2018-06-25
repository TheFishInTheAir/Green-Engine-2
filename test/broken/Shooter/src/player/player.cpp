/*
#include <Shooter/include/player/player.h>
#include "common/BasicLightModel.h"
#include "input/KeyboardHandler.h"
#include <math.h>

#define SPEED 0.5
#define TEMP_MODEL_SCALE 0.2

ge::GraphicsCore* Player::gc;

void Player::update()
{
	///Velocity Calculations

	vY += 0 > vY ? 0.15 : -0.15;
	vX += 0 > vX ? 0.15 : -0.15;

	vY = abs(vY) < 0.2 ? 0 : vY;
	vX = abs(vX) < 0.2 ? 0 : vX;

	///READ KEYS

	if (KeyboardHandler::keyDown(KeyboardKey::W))
	{
		vY += SPEED;
	}
	if (KeyboardHandler::keyDown(KeyboardKey::S))
	{
		vY -= SPEED;
	}
	if (KeyboardHandler::keyDown(KeyboardKey::D))
	{
		vX += SPEED;
	}
	if (KeyboardHandler::keyDown(KeyboardKey::A))
	{
		vX -= SPEED;
	}

	///MOVE PLAYER
	x += vX / 100;
	y += vY / 100;


	///COLLISION CHECK TODO




	///Bounds Check Bounce

	if (abs(x)>16-0.25)
	{
		vX = -vX;
		vX -= vX/2;

		x = 0 > x ? -(16 - 0.25) : 16 - 0.25;
	}

	if (abs(y)>16-0.25)
	{
		vY = -vY;
		vY -= vY/2;

		y = 0 > y ? -(16 - 0.25) : 16 - 0.25;

	}


	//Camera

	cam->pos = { -x,  5, y };
	cam->dir = { -x, -1, y };
	cam->up  = { 0, 0, 1 };
	cam->update();

	//Model
	tempModel->model = glm::scale(glm::translate(glm::mat4(1), { -x, 0, y }), glm::vec3(TEMP_MODEL_SCALE));

}

void Player::render()
{
	tempModel->render();
}

Player::Player()
{

	ge::GlobalRuntime::ge_REGISTER_RUNTIME_HANDLER;

	gc = ge::GlobalMemory::get("ge_renderer_instance").getRawData<ge::GraphicsCore>();

	int w, h;
	gc->window->getSize(&w,&h);

	cam = new Camera();
	cam->nearCull = 0.1f;
	cam->farCull = 100.0f;
	cam->aspectRatio = ((float)w) / ((float)h);
	cam->fov = glm::radians(45.0f);
	cam->up = glm::vec3(0, 1, 0);
	cam->pos = glm::vec3(0, 0, 0);
	cam->dir = glm::vec3(0, 0, -1);

	if (ge::GlobalMemory::exists(DBL_STRINGIFY(CURRENT_CAMERA)))
	{
		ConsoleIO::print("CURRENT CAMERA ALREADY DEFINED, REPLACING IT", MessageType::Warning);
		ge::GlobalMemory::remove(DBL_STRINGIFY(CURRENT_CAMERA));
	}
	ge::GlobalMemory::insert(DBL_STRINGIFY(CURRENT_CAMERA), { cam, ReadableMemType::OTHER });

	tempModel = new Debug::DebugColouredBox(cam, { 0,0,0 });
	tempModel->colour = {1,1,1};

	ge::GlobalMemory::get("ge_render_runtime_group").getRawData<ge::RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);
	ge::GlobalMemory::get("ge_update_runtime_group").getRawData<ge::RuntimeGroup>()->ge_RUNTIME_GROUP_INSERT_HEAP(this);

}
*/
