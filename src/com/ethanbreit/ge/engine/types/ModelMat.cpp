#include <ge/engine/types/ModelMat.h>

#include <glm/gtx/quaternion.hpp>


namespace ge
{
 //TODO: IMPROVE AND 
	void changeP(float** var, float* result)
	{
		*var = result;
	}
	
	ModelMat::ModelMat()
	{
			//changeP(&(&(matrix[1][1])), &(pos.x));
	}

	glm::mat4 ModelMat::getMat()
	{
		glm::mat4 sMat(1);
		glm::mat4 tMat(1);
		glm::mat4 rMat;

		tMat = glm::translate(tMat, pos);
		rMat = glm::toMat4(rot);
		sMat = glm::scale(sMat, scale);

		return tMat* rMat *sMat;
	}
}
