//
// Created by Ethan Breit on 2017-08-08.
//
#include <GL/glew.h>
#include <GLFW/glfw.h>

#pragma once
namespace ge
{
    namespace KeyType
    {
        typedef enum
        {
            GE_KEY_A = GLFW_KEY_A,
            GE_KEY_B = GLFW_KEY_B,
            GE_KEY_C = GLFW_KEY_C,
            GE_KEY_D = GLFW_KEY_D,
            GE_KEY_E = GLFW_KEY_E,
            GE_KEY_F = GLFW_KEY_F,
            GE_KEY_G = GLFW_KEY_G,
            GE_KEY_H = GLFW_KEY_H,
            GE_KEY_I = GLFW_KEY_I,
            GE_KEY_J = GLFW_KEY_J,
            GE_KEY_K = GLFW_KEY_K,
            GE_KEY_L = GLFW_KEY_L,
            GE_KEY_M = GLFW_KEY_M,
            GE_KEY_N = GLFW_KEY_N,
            GE_KEY_O = GLFW_KEY_O,
            GE_KEY_P = GLFW_KEY_P,
            GE_KEY_Q = GLFW_KEY_Q,
            GE_KEY_R = GLFW_KEY_R,
            GE_KEY_S = GLFW_KEY_S,
            GE_KEY_T = GLFW_KEY_T,
            GE_KEY_U = GLFW_KEY_U,
            GE_KEY_V = GLFW_KEY_V,
            GE_KEY_W = GLFW_KEY_W,
            GE_KEY_X = GLFW_KEY_X,
            GE_KEY_Y = GLFW_KEY_Y,
            GE_KEY_Z = GLFW_KEY_Z,
            GE_KEY_UP = GLFW_KEY_UP,
            GE_KEY_DOWN = GLFW_KEY_DOWN,
            GE_KEY_LEFT = GLFW_KEY_LEFT,
            GE_KEY_RIGHT = GLFW_KEY_RIGHT,
            GE_KEY_ESC = GLFW_KEY_ESCAPE,
            GE_KEY_SPACE = GLFW_KEY_SPACE,
            GE_KEY_LSHIFT = GLFW_KEY_LEFT_SHIFT,
            GE_KEY_RSHIFT = GLFW_KEY_RIGHT_SHIFT,
            GE_KEY_TAB = GLFW_KEY_TAB,
            GE_KEY_LCTRL = GLFW_KEY_LEFT_CONTROL,
            GE_KEY_TILDA = GLFW_KEY_GRAVE_ACCENT,
            GE_KEY_BACKSPACE = GLFW_KEY_BACKSPACE,
            GE_KEY_RETURN = GLFW_KEY_ENTER,
            //add in more


        } type;
    }
}