//
// Created by Ethan Breit on 2017-08-16.
//

#include <ge/graphics/enums/TextureFilterType.h>
#include <ge/graphics/enums/ColourModelType.h>
#include <ge/graphics/types/CubeMap.h>

namespace ge
{

    TextureFilterType::type CubeMap::getFiltering()
    {
        return filtering;
    }

    ColourModelType::type CubeMap::getColourModel()
    {
        return colourModel;
    }

    int CubeMap::getDimension() const
    {
        return dim;
    }

}