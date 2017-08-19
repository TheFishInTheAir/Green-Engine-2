//
// Created by Ethan Breit on 2017-08-16.
//

#include <loader/LoadImage.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
namespace ge
{
    namespace ImageLoader
    {

        ge::Error loadImage(std::string path, Image **out)
        {
            int width, height, nrChannels;
            const void *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);


            *out = new Image();
            (*out)->width = width;
            (*out)->height = height;

            (*out)->data = data;
            if(nrChannels==3)
                (*out)->model = ColourModelType::RGB;
            else
                (*out)->model = ColourModelType::RGBA;

            return ge::Error();
        }
    }
}