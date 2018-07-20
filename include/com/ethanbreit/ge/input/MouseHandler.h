//
// Created by Ethan Breit on 2017-08-08.
//

#pragma once
#include <ge/graphics/types/Window.h>
#include <GLFW/glfw3.h>
#include <functional>

namespace ge
{
    namespace MouseHandler
    {
	    inline bool moveMouse = false;
		inline bool centerMouse = false;
        //TODO: Add More Input Functions (I.E. Listeners and Mouse Buttons)
        void _mouseHandler(GLFWwindow* window, double xpos, double ypos);
        void getMousePos(double*,double *);
        void resetMousePos();


		//TODO: IMPLEMENT. replace uint32_t with mouse input enum, also make this platform independant; ALSO NOTE: no memory leaks because we actually can delete lambdas and not rely on GC.
		void insertMouseListiner(std::function<uint32_t>); 
    }
}