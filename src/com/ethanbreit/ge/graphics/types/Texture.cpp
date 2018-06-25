//
// Created by Ethan Breit on 2017-08-16.
//

#include <ge/graphics/enums/TextureFilterType.h>
#include <ge/graphics/enums/ColourModelType.h>
#include <ge/graphics/types/Texture.h>

namespace ge
{

    TextureFilterType::type Texture::getFiltering()
    {
        return filtering;
    }

    ColourModelType::type Texture::getColourModel()
    {
        return colourModel;
    }

    int Texture::getWidth() const
    {
        return width;
    }

    int Texture::getHeight() const
    {
        return height;
    }
}