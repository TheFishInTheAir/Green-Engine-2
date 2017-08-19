//
// Created by Ethan Breit on 2017-08-08.
//

#pragma once

#include "KeyboardKey.h"

namespace ge
{
    namespace KeyboardHandler
    {


        bool keyDown(KeyboardKey::type);
        bool keyDownSticky(KeyboardKey::type);

        void insertKEyListener(void(*)(KeyboardKey::type, int32_t osKeyCode, KeyboardModifier::type, KeyboardKeyState::type));

    }
}