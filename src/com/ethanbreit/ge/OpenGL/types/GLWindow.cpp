//
// Created by Ethan Breit on 2017-07-23.
//
#ifdef _WIN32
#define GLFW_DLL
#endif

#include <ge/graphics/abs/OpenGL/types/GLWindow.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/input/MouseHandler.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/graphics/abs/OpenGL/FeatureHandler.h>
#include <ge/graphics/GraphicsCore.h>
#include <iostream>
#include <ge/debug/PreProcessor.h>
#include <ge/util/PreprocessorUtil.h>
#include <ge/console/Log.h>

ge::GraphicsCore* core;

namespace ge
{
    namespace GL
    {
		const std::string LOG_TAG = "GLWindow";

        void glfwErrorCallback(int error, char* err)
        {
            Log::critErr(LOG_TAG, "GLFW Error: "+std::string(err));
        }

        void glfwResizeCallback(); /// TODO: implement @UNFINISHED

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
            _majorVersion        =      windowConstructorInfo.gl_major_version;
            _minorVersion        =      windowConstructorInfo.gl_minor_version;
            _isForwardCompatible =      windowConstructorInfo.is_forward_compatible;

            _windowName          =      windowConstructorInfo.window_name;
            _startThread         =      windowConstructorInfo.startup_thread;
            _hasDepthBuffer      =      windowConstructorInfo.has_depth_buffer;
            ///Initialise Clear Mask
            _clearMask           =      GL_COLOR_BUFFER_BIT;

			core				 =		GraphicsCore::ctx;



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


            Log::msg(LOG_TAG, "Finding available Monitors:");
            int count;
            GLFWmonitor **monitors = glfwGetMonitors(&count);
            for(int i = 0; i<count; i++)
            {
                Log::msg(LOG_TAG,
                        " - " + std::string(glfwGetMonitorName(
                                *(monitors + i)
                        )));
            }
            Log::msg(LOG_TAG, "Total of (" + std::to_string(count) + ") monitors found.");

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
            //glfwWindowHint(GLFW_MAXIMIZED, true);


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

				ge_DEBUG_TIMER_START;


            //glfwSetWindowSizeCallback(_window,(GLFWwindowsizefun)glfwResizeCallback); /// @unimplemented Set the GLFW Window Resize Callback to the glfwResizeCallback() function
            
			if (windowConstructorInfo.hidden_cursor)
			{
				glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Fuuuuuuck, this has to be called  on the main thread
			}
        	else
			{
				glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

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
			{
				int w,  h;
				getSize(&w, &h);
				glViewport(0, 0, w, h); /// Not really part of glew init
			}

            glewExperimental= (GLboolean) true; /// Needed in core profile
            if (glewInit() != GLEW_OK) {
                ge_Error_GENERATE("GLEW Failed to Initialize");
            }

			ge_DEBUG_TIMER_END("GLEW Init (E not f)")

			setClearColour(windowConstructorInfo.clear_colour);
			glDepthFunc(GL_LEQUAL);

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

            glfwSwapInterval(0); //Disable V-SYNC

            //glfwSetCursorPos(_window,0,0);

			/**
			 *
			 * Release Context
			 *
			 */
			//glfwMakeContextCurrent(_window);
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
            
//			GlobalMemory::insert("ge_render_context_runtime", GlobalMemory::MemItem(r, ReadableMemType::OTHER));
			glfwMakeContextCurrent(nullptr);

            glfwMakeContextCurrent(_window);
        }
		

    }
}
