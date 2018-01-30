//
// Created by Ethan Breit on 2017-07-23.
//
#ifdef _WIN32
#define GLFW_DLL
#endif

#include <OpenGL/types/GLWindow.h>
#include <input/KeyboardHandler.h>
#include <input/MouseHandler.h>
#include "memory/GlobalMemory.h"
#include "OpenGL/FeatureHandler.h"
#include <Graphics/GraphicsCore.h>
#include <iostream>
#include "debug/PreProcessor.h"

ge::GraphicsCore* core;

namespace ge
{
    namespace GL
    {

        void glfwErrorCallback(int error, char* err)
        {
            ConsoleIO::print("GLFW Error: "+std::string(err)+"\n", MessageType::Error);
        }

        void glfwResizeCallback(); /// @unimplemented TODO: implement

        Window::Window()
        {

        }

        Window::~Window()
        {
            cleanup();
        }

        Error Window::init(WindowConstructorInfo windowConstructorInfo)
        {
/**
             *
             * Setup Variables
             *
             */

            ///Extract variables from constructor info
            _width               =      windowConstructorInfo.width;
            _height              =      windowConstructorInfo.height;

            _samples             =      windowConstructorInfo.samples;
            _majorVersion        =      windowConstructorInfo.glMajorVersion;
            _minorVersion        =      windowConstructorInfo.glMinorVersion;
            _isForwardCompatible =      windowConstructorInfo.isForwardCompatible;

            _windowName          =      windowConstructorInfo.windowName;
            _startThread         =      windowConstructorInfo.startupThread;
            _hasDepthBuffer      =      windowConstructorInfo.hasDepthBuffer;
            ///Initialise Clear Mask
            _clearMask           =      GL_COLOR_BUFFER_BIT;

			core				 =		GlobalMemory::get("ge_renderer_instance").getRawData<GraphicsCore>();



            /**
             *
             * GLFW Initialisation
             *
             */
			ge_DEBUG_TIMER_INIT

			ge_DEBUG_TIMER_START

            if (!glfwInit())
                ge_Error_GENERATE("GLFW failed to initialize!");

            glfwSetErrorCallback((GLFWerrorfun)glfwErrorCallback); ///Set the GLFW Error Callback to the glfwErrorCallback() function

			ge_DEBUG_TIMER_END("GLFW INIT")

            /**
             *
             * Test for Monitors
             *
             */

			ge_DEBUG_TIMER_START


            ConsoleIO::print("Finding available Monitors:\n");
            int count;
            GLFWmonitor **monitors =glfwGetMonitors(&count);
            for(int i = 0; i<count; i++)
            {
                ConsoleIO::print(
                        std::string(glfwGetMonitorName(
                                *(monitors + i)
                        )) + "\n");
            }
            ConsoleIO::print("Total of (" + std::to_string(count) + ") monitors found.\n");

			ge_DEBUG_TIMER_END("GLFW Monitor Test")


            /**
             *
             * GLFW Window Initialization
             *
             */
			ge_DEBUG_TIMER_START

            glfwWindowHint(GLFW_SAMPLES, _samples); // antialiasing
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _majorVersion); // We want OpenGL 3.3
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minorVersion);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, _isForwardCompatible); // To make MacOS happy; should not be needed
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
            glfwWindowHint(GLFW_MAXIMIZED, true);



			ge_DEBUG_TIMER_END("GLFW Window Hinted")


			ge_DEBUG_TIMER_START

            //_window = glfwCreateWindow( _width, _height, _windowName, glfwGetPrimaryMonitor(), NULL); ///Start Fullscreen
            _window = glfwCreateWindow( _width, _height, _windowName, NULL, NULL); ///Start Windowed
            if( _window == NULL ){
                glfwTerminate();
                ge_Error_GENERATE("GLFW failed to create window. (Possible incompatible OpenGL driver)");
            }

			ge_DEBUG_TIMER_END("GLFW Window Creation")


            /**
             *
             * GLFW Input Setup
             *
             */

			ge_DEBUG_TIMER_START


            //glfwSetWindowSizeCallback(_window,(GLFWwindowsizefun)glfwResizeCallback); /// @unimplemented Set the GLFW Window Resize Callback to the glfwResizeCallback() function
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            //glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE); ///Maybe don't enable this TODO: add proper window input handler
            glfwSetKeyCallback(_window, KeyboardHandler::_keyHandler);
            glfwSetCursorPosCallback(_window, MouseHandler::_mouseHandler);

			ge_DEBUG_TIMER_END("GLFW Input Init")


            /**
             *
             * GLEW Init
             *
             */

			ge_DEBUG_TIMER_START


            glfwMakeContextCurrent(_window); /// Initialize GLEW
            glewExperimental= (GLboolean) true; /// Needed in core profile
            if (glewInit() != GLEW_OK) {
                ge_Error_GENERATE("GLEW Failed to Initialize\n");
            }

			ge_DEBUG_TIMER_END("GLEW Init (E not f)")


			ge::GL::registerFeatures(&((*core).supportedFeatures));




            /**
             *
             * OpenGL Configuration
             *
             */

            if(_hasDepthBuffer)
            {
                glEnable(GL_DEPTH_TEST);
                _clearMask |= GL_DEPTH_BUFFER_BIT;
            }


            glfwSetCursorPos(_window,0,0);

			/**
			 *
			 * Release Context
			 *
			 */
			glfwMakeContextCurrent(nullptr);

            return Error();
        }

        std::string Window::getWindowName(void)
        {
            return std::string();
        }

        void Window::poll()
        {
            glfwPollEvents();
        }

        void Window::swap()
        {
            glfwSwapBuffers(_window);
        }

        void Window::cleanup()
        {
            glfwTerminate();
        }

        bool Window::shouldClose()
        {
            return (bool) glfwWindowShouldClose(_window);
        }

        void Window::getSize(int *w, int *h)
        {
            glfwGetWindowSize(_window, w, h);
        }

        void Window::setClearColour(glm::vec3 c)
        {
            glClearColor(c.x,c.y,c.z, 1);
        }

        void Window::clear()
        {
            glClear( _clearMask );
        }

        void Window::makeCurrentThread(Runtime* r)
        {
			GlobalMemory::insert("ge_render_context_runtime", GlobalMemory::MemItem(r, ReadableMemType::OTHER));
            glfwMakeContextCurrent(_window);
        }


    }
}
