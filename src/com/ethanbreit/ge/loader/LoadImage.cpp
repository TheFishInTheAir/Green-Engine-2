//
// Created by Ethan Breit on 2017-08-16.
//

#include <ge/loader/LoadImage.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <ge/util/ResourceUtil.h>
#include <ge/console/ConsoleIO.h>
#include <memory>

namespace ge
{
	namespace ImageLoader
	{

		ge::Error loadImage(std::string path, Image **out) //TODO: we only need a pointer not a pointer to a pointer @REFACTOR
		{
			int width, height, nrChannels;
			const void *data = stbi_load(ResourceUtil::getResPath(path).c_str(), &width, &height, &nrChannels, 0);

			if (data == nullptr)
			{
				std::string error;
				error += "Error Loading Image: ";
				error += path;
				error += ", STB Image error: \n";
				error += stbi_failure_reason();
				ge_Error_GENERATE(error);
			}

			*out = new Image();
			(*out)->width = width;
			(*out)->height = height;
            
            (*out)->url = path;

			(*out)->data = data;
			if (nrChannels == 3)
			{
				(*out)->model = ColourModelType::RGB;
			}
			else if(nrChannels == 1)
			{
				(*out)->model = ColourModelType::BW;
			}
			else
			{
				(*out)->model = ColourModelType::RGBA;
			}
			return ge::Error();
		}
		ge::Error loadImage(std::string path, std::shared_ptr<Image> *outop)
		{
			int width, height, nrChannels;
			const void *data = stbi_load(ResourceUtil::getResPath(path).c_str(), &width, &height, &nrChannels, 0);

			if (data == nullptr)
			{
				std::string error;
				error += "Error Loading Image: ";
				error += path;
				error += ", STB Image error: \n";
				error += stbi_failure_reason();
				ge_Error_GENERATE(error);
			}

			Image *out = new Image();
			(out)->width = width;
			(out)->height = height;
            
            out->url = path;
            
			(out)->data = data;
			if (nrChannels == 3)
				(out)->model = ColourModelType::RGB;
			else
				(out)->model = ColourModelType::RGBA;

			*outop = std::shared_ptr<Image>(out);

			return ge::Error();
		}

		/*Error loadImage(std::string p, std::shared_ptr<Image>* ptr, ge::Scene* s)
		{
			Error err = loadImage(p, ptr);
			s->images.push_front(*ptr);
			ge_Error_ADDTRACE(err);
			return err;
		}*/
	}
}
