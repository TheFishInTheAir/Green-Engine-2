#include <ge/engine/scene/Scene.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/entity/EntitySerial.h>
#include <engine/global_settings.pre>
#include <ge/engine/defaults/StaticObject.h>
#include <ge/entity/component/components/MeshRendererComponent.h>
#include <ge/loader/LoadShader.h>
#include <ge/console/Log.h>
#include <ge/default_geom/Cube.h>
#include <ge/graphics/types/Uniform.h>
#include <ge/entity/EntityManager.h>
namespace ge
{
    Scene* Scene::currentScene = nullptr;
    
    const std::string LOG_TAG = "Scene";

    
	void Scene::loadScene(Empty::Scene s)
	{
        GraphicsCore* gc = GraphicsCore::ctx;

		url = s.url;
		//Clear
		{

			std::vector<std::pair<std::string, std::shared_ptr<ge::Texture>>> texturesK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::CubeMap>>> cubemapsK;
			//std::vector<std::pair<std::string, ge::Material>> 				  materialK;

			std::vector<std::pair<std::string, std::shared_ptr<ge::Empty::MeshData>>> meshesK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::ShaderGroup>>> shadersK;
			std::vector<std::pair<std::string, std::shared_ptr<ge::Audio::AudioClip>>> audioClipsK;
			
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

				auto ac = audioClips.find(res);
				if (ac != audioClips.end())
				{
					audioClipsK.push_back({ ac->first, ac->second });
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
			audioClips.clear();
			for (auto ac : audioClipsK)
				audioClips.insert({ ac.first, ac.second });


			materials.clear(); //NOTE: materials are so small we dont really care that they don't get kept...
			directionalLights.resize(LIGHT_DIR_MAX);
			directionalLights.clear();
			pointLights.resize(LIGHT_POINT_MAX);
			pointLights.clear();
			spotLights.resize(LIGHT_SPOT_MAX);
			spotLights.clear();
			//directionalLights.(nullptr);
		}

		//Textures
	
		for (auto img : s.images)
		{
			Texture* tex;
			gc->textureFactory->genTexture(img.second, &tex);
			

			//TODO: add a proper global state for default texture filter, but I am tired of setting this everytime.
			tex->setFiltering(TextureFilterType::Anisotropic_16x);



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

		//AudioClips

		for (auto ac : s.audioClips)
		{
			ac->upload();
			audioClips.insert({ ac->url, std::shared_ptr<Audio::AudioClip>(ac) });
		}
		//Materials

		for(auto m : s.materials)
		{
			materials.insert({m.name, m});
			Log::tVrb(LOG_TAG, "Loaded Material '"+m.name+"'");
		}
		
		//Skybox

		if (s.skybox != "")
		{
			Log::msg("Setting SkyBox to " + s.skybox);

			if(!cubemaps.count(s.skybox))
			{
				Log::err("Incorrect SkyBox Name");
			}

			//Do not use Gloable Memory
            skybox = new Entity();
            
			skybox->name = "skybox";
			skybox->registerToTag(EntityManager::getOrCreateTag("no_export"));

            MeshRendererComponent* mrc = new MeshRendererComponent(skybox, GraphicsCore::ctx->meshFactory->newTriangleMesh(ge::dgeo::cube::getMeshData()));
            
            mrc->mesh->setShaderGroup(shaderGroups["engine/defaults/forward/skybox/skybox.gesm"].get()); //NOTE: TEMP
            ge::Uniform::UniformContent uc;
            uc.iv1 = 0;

            mrc->mesh->setUniform(DBL_STRINGIFY(SKYBOX), uc);
			mrc->mesh->registerCubeMap(cubemaps[s.skybox].get(), 0);
			mrc->mesh->cullBackface = false;
			cubemaps[s.skybox].get()->setFiltering(TextureFilterType::Anisotropic_16x);
			skyboxCubemapName = s.skybox;

			//cubemaps[DBL_STRINGIFY(SKYBOX)] = cubemaps[s.skybox];

			mrc->mesh->rebuffer();
            mrc->insertToDefaultBatch();
			skybox->insertComponent(mrc);
			EntityManager::registerEntity(skybox);
            
		}

		//Entities
		for(auto e : s.ents)
		{
			Entity* tEnt = EntitySerial::deserializeEntity(e);
			for(auto c : tEnt->components)
				c.second->insertToDefaultBatch();
			EntityManager::registerEntity(tEnt);
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
