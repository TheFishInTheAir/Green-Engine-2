//
// Created by Ethan Breit on 2017-08-16.
//

#pragma once

#include <graphics/enums/TextureFilterType.h>
#include <graphics/enums/ColourModelType.h>
#include <error/Error.h>

namespace ge
{
    struct Texture
    {

        virtual TextureFilterType::type getFiltering() = 0;
        virtual ColourModelType::type getColourModel() = 0;
        virtual Error setFiltering(TextureFilterType::type) = 0;

    private:

        TextureFilterType   ::type filtering;
        ColourModelType     ::type colourModel;

    };
}