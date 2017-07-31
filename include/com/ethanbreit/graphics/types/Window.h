//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <string>
#include <error/Error.h>
#include <glm/vec3.hpp>

namespace ge
{

    struct WindowConstructorInfo
    {
        int width                   = 500           ;
        int height                  = 500           ;
        int samples                 = 4             ;
        int glMajorVersion          = 3             ;
        int glMinorVersion          = 3             ;
        int mtlMajorVersion         = 3             ; ///Don't know if this is necessary
        int mtlMinorVersion         = 3             ; ///Don't know if this is necessary
        int vkMajorVersion          = 3             ; ///Don't know if this is necessary
        int vkMinorVersion          = 3             ; ///Don't know if this is necessary
        bool isForwardCompatible    = true          ;
        const char *windowName      = "NULL"        ;
    };


    struct Window
    {
        //virtual Window(WindowConstructorInfo) = 0;

        //virtual ~Window() = 0;

        virtual Error init(WindowConstructorInfo) = 0;

        virtual std::string getWindowName(void) = 0;

        virtual void poll() = 0;

        virtual void swap() = 0;

        virtual void cleanup() = 0;

        virtual void setClearColour(glm::vec3) = 0;

        virtual void clear() = 0;

        virtual bool shouldClose() = 0;

        virtual void getSize(int *, int *) = 0;
    };
}