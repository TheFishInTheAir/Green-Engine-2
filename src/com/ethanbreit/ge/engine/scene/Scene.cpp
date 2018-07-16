#include <ge/engine/scene/Scene.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <engine/global_settings.pre>
#include <ge/engine/defaults/StaticObject.h>
#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/loader/LoadShader.h>
#include <ge/console/Log.h>
#include <ge/default_geom/Cube.h>
#include <ge/graphics/types/Uniform.h>
namespace ge
{
    Scene* Scene::currentScene = nullptr;
    
    const std::string LOG_TAG = "Scene";

    
	void Scene::loadScene(Empty::Scene s)
	{
        GraphicsCore* gc = GraphicsCore::ctx;

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
				auto shader = shaderGroups.find(res);
				if (shader != shaderGroups.end())
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
				shaderGroups.insert({ s.first,s.second });
		}

		//Textures
	
		for (auto img : s.images)
		{
			Texture* tex;
			gc->textureFactory->genTexture(img.second, &tex);

			textures.insert({ img.first, std::shared_ptr<Texture>(tex) });
            Log::tVrb(LOG_TAG, "Loaded Texture '"+img.first+"'");
		}

		//Cubemaps

		for (auto cmap : s.cubemaps)
		{
			ge::CubeMap* realCMap;
			gc->textureFactory->genCubeMap(cmap.second, &realCMap);

			cubemaps.insert({ cmap.first, std::shared_ptr<CubeMap>(realCMap) });
            Log::tVrb(LOG_TAG, "Loaded Cubemap '"+cmap.first+"'");
		}


		//Meshes

		
		for (auto meshdat : s.meshes)
		{
			Empty::MeshData* md = new Empty::MeshData;
			*md = meshdat.second;
			meshes.insert({ meshdat.first, std::shared_ptr<ge::Empty::MeshData> ( md )});
            Log::tVrb(LOG_TAG, "Loaded Mesh    '"+meshdat.first+"'");

		}

		
		//Shaders

		for (auto shaderManifest : s.shaders)
		{
			ShaderGroup* sg;
			ShaderLoader::loadShader(shaderManifest, &sg, this);
			shaderGroups.insert({shaderManifest, std::shared_ptr<ShaderGroup>(sg)});
            Log::tVrb(LOG_TAG, "Loaded Shader '"+shaderManifest+"'");

		}
		

		if (s.skybox != "")
		{
			ConsoleIO::print("Setting SkyBox to " + s.skybox + "\n");

			if(!cubemaps.count(s.skybox))
			{
				ConsoleIO::print("Incorrect SkyBox Name", MessageType::Error);
			}

			//Do not use Gloable Memory

            skybox = new Entity();
            
            MeshRendererComponent* mrc = new MeshRendererComponent(skybox, GraphicsCore::ctx->meshFactory->newTriangleMesh(ge::dgeo::cube::getMeshData()));
            
            mrc->mesh->setShaderGroup(shaderGroups["engine/defaults/skybox/skybox.gesm"].get());
            ge::Uniform::UniformContent uc;
            uc.iv1 = 0;
            mrc->mesh->setUniform("CUBEMAP_0", uc);
            
            mrc->insertToDefaultBatch();
            
		}

		//Initialise Static Objects
		for (Empty::StaticObject staticObjectE : s.staticObjects)
		{
            StandardEntGen::genStaticObject(staticObjectE); //TODO: THIS DOESN'T WORK, I MEAN IT DOES BUT DON'T USE IT
		}

	}

	void Scene::instantiateScene()
	{
	}
}
