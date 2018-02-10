//
// Created by Ethan Breit on 2017-08-10.
//
#include <input/MouseHandler.h>
#include <console/ConsoleIO.h>

namespace ge
{
    namespace MouseHandler
    {
        double x = 0,y = 0;

        void getMousePos(double* xIn,double* yIn)
        {
            *xIn = x;
            *yIn = y;
        }
        void resetMousePos()
        {
            x = 0;
            y = 0;
        }

        void _mouseHandler(GLFWwindow *window, double xpos, double ypos)
        {

			static bool initialised = false;

			static int halfWidth;
			static int halfHeight;

			if (!initialised)
			{
				initialised = true;

				glfwGetWindowSize(window, &halfWidth, &halfHeight);
				halfWidth /= 2;
				halfHeight /= 2;

			}

			if (!moveMouse)
			{
				
				glfwSetCursorPos(window, centerMouse ? halfWidth : 0, centerMouse ? halfHeight : 0);
			}
            x += centerMouse ? xpos - halfWidth : xpos;
            y += centerMouse ? ypos - halfHeight: ypos;
        }
    }
}
