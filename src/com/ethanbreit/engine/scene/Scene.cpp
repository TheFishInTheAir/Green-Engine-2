#include <engine/scene/Scene.h>
#include "graphics/GraphicsCore.h"
#include <memory/GlobalMemory.h>
#include <engine/global_settings.pre>
#include "engine/defaults/StaticObject.h"

namespace ge
{
	void Scene::loadScene(Empty::Scene s)
	{
		GraphicsCore* gc = GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>(); //TODO: replace string with MACRO
		

		//Clear
		{

			std::vector<std::pair<std::string, std::shared_ptr<ge::Texture>>> texturesK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::CubeMap>>> cubemapsK;

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

				auto cmap = cubemaps.find(res);
				if (cmap != cubemaps.end())
				{
					cubemapsK.push_back({ cmap->first,cmap->second });
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
			cubemaps.clear();
			for (auto cmap : cubemapsK)
				cubemaps.insert({ cmap.first,cmap.second });
			meshes.clear();
			for (auto m : meshesK)
				meshes.insert({ m.first,m.second });
			shaders.clear();
			for (auto s : shadersK)
				shaders.insert({ s.first,s.second });
		}


		//Textures
	
		for (auto img : s.images)
		{
			Texture* tex;
			gc->textureFactory->genTexture(img.second, &tex);

			textures.insert({ img.first, std::shared_ptr<Texture>(tex) });
		}

		//Cubemaps

		for (auto cmap : s.cubemaps)
		{
			ge::CubeMap* realCMap;
			gc->textureFactory->genCubeMap(cmap.second, &realCMap);

			cubemaps.insert({ cmap.first, std::shared_ptr<CubeMap>(realCMap) });
		}


		//Meshes

		
		for (auto meshdat : s.meshes)
		{
			Empty::MeshData* md = new Empty::MeshData;
			*md = meshdat.second;
			meshes.insert({ meshdat.first, std::shared_ptr<ge::Empty::MeshData> ( md )});

		}

		/*
		//Shaders
		//TODO: create shader Manifest's soon pls... pls...
		
		for (auto shaderSource : s.shaders)
		{
			Texture* tex;
			gc->textureFactory->genTexture(img.second, &tex);

			textures.insert({ img.first, std::shared_ptr<Texture>(tex) });
		}
		*/

		if (s.skybox != "")
		{
			ConsoleIO::print("Setting SkyBox to " + s.skybox + "\n");

			if(!cubemaps.count(s.skybox))
			{
				ConsoleIO::print("Incorrect SkyBox Name", MessageType::Error);
			}

			if (GlobalMemory::exists(MSTR(CURRENT_SKYBOX)))
			{
				SkyBox* sb = GlobalMemory::get(MSTR(CURRENT_SKYBOX)).getRawData<SkyBox>();
				delete sb;

				GlobalMemory::insert(MSTR(CURRENT_SKYBOX), { new SkyBox(cubemaps.find(s.skybox)->second.get()),ReadableMemType::OTHER });

			}
			else
			{
				GlobalMemory::insert(MSTR(CURRENT_SKYBOX), { new SkyBox(cubemaps.find(s.skybox)->second.get()),ReadableMemType::OTHER });
			}
		}


		std::forward_list<StaticObject*>* objsTemp = GlobalMemory::get(MSTR(GE_ENTITES_GM)).getRawData<std::forward_list<StaticObject*>>();

		//Initialise Static Objects

		for(Empty::StaticObject staticObjectE : s.staticObjects)
		{
			objsTemp->push_front(new StaticObject(staticObjectE));

		}

	}



	void Scene::init()
	{
		if(!GlobalMemory::exists(MSTR(GE_ENTITES_GM)))
		{
			//TODO: proper stuff (remove and make good)
			std::forward_list<StaticObject*>* objsTemp = new std::forward_list<StaticObject*>();
			
			GlobalMemory::insert(MSTR(GE_ENTITES_GM), {objsTemp, ReadableMemType::OTHER});


		}
	}
}
