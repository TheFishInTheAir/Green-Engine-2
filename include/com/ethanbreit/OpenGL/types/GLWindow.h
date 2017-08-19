//
// Created by Ethan Breit on 2017-07-22.
//

#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <string>
#include <console/ConsoleIO.h>

#include <graphics/types/Window.h>

namespace ge
{
    namespace GL
    {

        struct Window : ge::Window
        {
            Window();

            ~Window();

            Error init(WindowConstructorInfo);

            std::string getWindowName(void);

            void poll();

            void swap();

            void cleanup();

            void makeCurrentThread();

            bool shouldClose();

            void setClearColour(glm::vec3);

            void clear();

            void getSize(int *, int *);
        private:

            int _width;
            int _height;
            int _samples;
            int _majorVersion;
            int _minorVersion;
            int _isForwardCompatible;
            unsigned int _clearMask;
            bool _hasDepthBuffer;
            const char *_windowName;
            GLFWwindow *_window;
            Runtime *_startThread;
        };
    }
}