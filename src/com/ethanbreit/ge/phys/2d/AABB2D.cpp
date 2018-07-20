#include <ge/phys/2d/AABB2D.h>

namespace ge
{
	AABB2D::AABB2D(glm::vec2 c, glm::vec2 e)
	{
		center = c;
		extents = e;
	}

	bool AABB2D::collidesWith(AABB2D aabb)
	{
		if (glm::abs(center.x - aabb.center.x) < extents.x + aabb.extents.x)
		{
			if (glm::abs(center.y - aabb.center.y) < extents.y + aabb.extents.y)
			{
				return true;
			}
		}

		return false;
	}

	bool AABB2D::inside(glm::vec2 vec)
	{
		if (glm::abs(center.x - vec.x) < extents.x)
		{
			if (glm::abs(center.y - vec.y) < extents.y)
			{
				return true;
			}
		}
		return false;
	}
}