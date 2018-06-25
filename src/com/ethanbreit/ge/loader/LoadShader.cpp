//
// Created by Ethan Breit on 2017-12-23.
//
#include <ge/loader/LoadShader.h>

#include <ge/console/ConsoleIO.h>
#include <ge/util/ResourceUtil.h>
#include "json/json.hpp"
#include <ge/graphics/types/Shader.h>
#include <ge/graphics/GraphicsCore.h>
#include <ge/memory/GlobalMemory.h>
#include <engine/global_settings.pre>
#include <ge/engine/scene/Scene.h>
#include <ge/util/PreprocessorUtil.h>
#ifdef _WIN32

#define DIR_SEP '\\'

#else

#define DIR_SEP '/'

#endif

namespace ge
{

    namespace ShaderLoader
    {
		

        Error loadShader(std::string path, ShaderGroup** outShaderGroup)
        {
			static GraphicsCore* gc = GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getRawData<GraphicsCore>();

            ConsoleIO::print("Loading shader :"+path+"\n", MessageType::Verbose);

			nlohmann::json manifest;

			{
				std::string data;
				ResourceUtil::getRawStrResource(path, &data);
				manifest = nlohmann::json::parse(data.c_str());
			}
			
			std::vector<std::shared_ptr<Shader>> shaders;


			if(manifest.count("vertex"))
			{
				std::shared_ptr<Shader> vert;

				std::string vertData;
				ResourceUtil::getRawStrResource(manifest["vertex"], &vertData);
				gc->shaderFactory->genShader(vertData, ShaderType::Shader_Vertex, &vert);

				shaders.push_back(vert);
			}
			


			if (manifest.count("geometry"))
			{
				std::shared_ptr<Shader> geom;

				std::string geomData;
				ResourceUtil::getRawStrResource(manifest["geometry"], &geomData);
				gc->shaderFactory->genShader(geomData, ShaderType::Shader_Geometry, &geom);

				shaders.push_back(geom);
			}


			if (manifest.count("fragment"))
			{
				std::shared_ptr<Shader> frag;

				std::string fragData;
				ResourceUtil::getRawStrResource(manifest["fragment"], &fragData);
				gc->shaderFactory->genShader(fragData, ShaderType::Shader_Fragment, &frag);

				shaders.push_back(frag);
			}
			
			ShaderGroup *shaderGroup;

			gc->shaderFactory->genShaderGroup(shaders, &shaderGroup);

			*outShaderGroup = shaderGroup;

            return Error();
        }

	    Error loadShader(std::string path, ShaderGroup** outShaderGroup, Scene* s)
	    {
			static GraphicsCore* gc = GlobalMemory::get(DBL_STRINGIFY(RENDERER_INSTANCE)).getRawData<GraphicsCore>();

			ConsoleIO::print("Loading shader :" + path + "\n", MessageType::Verbose);

			nlohmann::json manifest;

			{
				std::string data;
				ResourceUtil::getRawStrResource(path, &data);
				manifest = nlohmann::json::parse(data.c_str());
			}

			std::vector<std::shared_ptr<Shader>> shaders;


			if (manifest.count("vertex"))
			{
				if(s->shaders.count(manifest["vertex"])>0)
				{
					shaders.push_back(s->shaders.at(manifest["vertex"]));
				}
				else
				{
					std::shared_ptr<Shader> vert;

					std::string vertData;
					ResourceUtil::getRawStrResource(manifest["vertex"], &vertData);
					gc->shaderFactory->genShader(vertData, ShaderType::Shader_Vertex, &vert);

					s->shaders.insert({manifest["vertex"], vert});

					shaders.push_back(vert);
				}
			}



			if (manifest.count("geometry"))
			{
				if (s->shaders.count(manifest["geometry"])>0)
				{
					shaders.push_back(s->shaders.at(manifest["geometry"]));
				}
				else
				{
					std::shared_ptr<Shader> geom;

					std::string geomData;
					ResourceUtil::getRawStrResource(manifest["geometry"], &geomData);
					gc->shaderFactory->genShader(geomData, ShaderType::Shader_Geometry, &geom);

					s->shaders.insert({ manifest["geometry"], geom });


					shaders.push_back(geom);
				}
			}


			if (manifest.count("fragment"))
			{
				if (s->shaders.count(manifest["fragment"])>0)
				{
					shaders.push_back(s->shaders.at(manifest["fragment"]));
				}
				else
				{
					std::shared_ptr<Shader> frag;

					std::string fragData;
					ResourceUtil::getRawStrResource(manifest["fragment"], &fragData);
					gc->shaderFactory->genShader(fragData, ShaderType::Shader_Fragment, &frag);

					s->shaders.insert({ manifest["fragment"], frag });

					shaders.push_back(frag);
				}
			}

			ShaderGroup *shaderGroup;

			gc->shaderFactory->genShaderGroup(shaders, &shaderGroup);

			*outShaderGroup = shaderGroup;

			return Error();
	    }
    }
}