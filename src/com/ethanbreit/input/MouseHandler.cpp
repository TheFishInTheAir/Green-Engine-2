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
            //ConsoleIO::print(std::to_string(xpos)+", "+std::to_string(ypos)+"\n");

            glfwSetCursorPos(window,0,0);
            x = xpos;
            y = ypos;
        }
    }
}
