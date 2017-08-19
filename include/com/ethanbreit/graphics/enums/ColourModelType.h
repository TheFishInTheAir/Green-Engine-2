//
// Created by Ethan Breit on 2017-08-16.
//

#pragma once
namespace ge
{
    namespace ColourModelType
    {
        typedef enum
        {

            RGB     = 0x0000,
            BGR     = 0x0001,
            ARGB    = 0x1002,
            RGBA    = 0x1003,
            ABGR    = 0x1004,
            BGRA    = 0x1005,
            /*CMYK,
            ACMYK,
            CMYKA,*/

        } type;

        //TODO: function to check if model has alpha

    }
}