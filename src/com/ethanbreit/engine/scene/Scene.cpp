#include <engine/scene/Scene.h>
#include "graphics/GraphicsCore.h"
#include <memory/GlobalMemory.h>
namespace ge
{
	void Scene::loadScene(Empty::Scene s)
	{
		GraphicsCore* gc = GlobalMemory::get("ge_renderer").getRawData<GraphicsCore>();
		
		//Textures
		
		{
			std::vector<std::string> texturesKept;

			std::vector<std::pair<std::string, std::shared_ptr<Texture>>> texturesIn;

			for (auto img : s.images) //O(n)
			{
				if (textures.count(img.first) != 0)
				{
					texturesKept.push_back(img.first);
					continue;
				}

				Texture* tex;
				gc->textureFactory->genTexture(img.second, &tex);
			}



		}
	}
}
