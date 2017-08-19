//
// Created by Ethan Breit on 2017-08-16.
//

#pragma once

#include <error/Error.h>
#include <graphics/types/Image.h>

namespace ge
{
    namespace ImageLoader
    {
        Error loadImage(std::string, Image**);
    }
}