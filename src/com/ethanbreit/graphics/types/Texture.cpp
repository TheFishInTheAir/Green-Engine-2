//
// Created by Ethan Breit on 2017-08-16.
//

#include <graphics/enums/TextureFilterType.h>
#include <graphics/enums/ColourModelType.h>
#include <graphics/types/Texture.h>

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