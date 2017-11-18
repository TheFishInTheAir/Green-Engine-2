#include <engine/types/ModelMat.h>

namespace ge
{

		void changeP(float** var, float* result)
		{
			*var = result;
		}
	
		ModelMat::ModelMat()
		{
			//changeP(&(&(matrix[1][1])), &(pos.x));
		}
}
