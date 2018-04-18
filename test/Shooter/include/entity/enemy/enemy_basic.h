#pragma once
#include "Shooter/include/entity/entity.h"
#include "boost/preprocessor/arithmetic/detail/div_base.hpp"

struct BasicEnemy : Entity
{
	BasicEnemy(float x, float y);
	void render();
	void update();
};

