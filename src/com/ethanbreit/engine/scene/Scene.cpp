#include <engine/scene/Scene.h>
#include "graphics/GraphicsCore.h"
#include <memory/GlobalMemory.h>
namespace ge
{
	void Scene::loadScene(Empty::Scene s)
	{
		GraphicsCore* gc = GlobalMemory::get("ge_renderer").getRawData<GraphicsCore>();
		

		//Clear
		{

			std::vector<std::pair<std::string, std::shared_ptr<ge::Texture>>> texturesK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::Empty::MeshData>>> meshesK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::ShaderGroup>>> shadersK;
			
			for(auto res : s.keptRes)
			{
				auto tex = textures.find(res);
				if (tex != textures.end())
				{
					texturesK.push_back({tex->first,tex->second});
					continue;
				}
				auto mesh = meshes.find(res);
				if (mesh != meshes.end())
				{
					meshesK.push_back({ mesh->first,mesh->second });
					continue;
				}
				auto shader = shaders.find(res);
				if (shader != shaders.end())
				{
					shadersK.push_back({ shader->first,shader->second });
					continue;
				}
			}

			textures.clear();
			for (auto t : texturesK)
				textures.insert({t.first,t.second});
			meshes.clear();
			for (auto m : meshesK)
				meshes.insert({ m.first,m.second });
			shaders.clear();
			for (auto s : shadersK)
				shaders.insert({ s.first,s.second });
		}


		//Textures
		
		{
			std::vector<std::pair<std::string, std::shared_ptr<Texture>>> texturesIn;

			for (auto img : s.images)
			{
				Texture* tex;
				gc->textureFactory->genTexture(img.second, &tex);
			}





		}
	}
}
