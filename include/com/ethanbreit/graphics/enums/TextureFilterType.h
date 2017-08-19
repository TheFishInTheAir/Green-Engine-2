//
// Created by Ethan Breit on 2017-08-16.
//

#pragma once
namespace ge
{
    namespace TextureFilterType
    {
        typedef enum
        {
            /// first digit is type, last digit is properties
            Nearest_Neighbour   = 0x00,
            Bilinear            = 0x10,
            Trilinear           = 0x11,
            Anisotropic_4x      = 0x21, ///last digit multiplied by 4 is level anisotropy
            Anisotropic_8x      = 0x22,
            Anisotropic_16x     = 0x34,
        } type;
    }
}