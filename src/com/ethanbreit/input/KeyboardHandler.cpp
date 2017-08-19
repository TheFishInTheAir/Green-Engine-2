//
// Created by Ethan Breit on 2017-08-08.
//

#import <input/KeyboardHandler.h>
#include <vector>

namespace ge
{
    namespace KeyboardHandler
    {

        /**
         *
         * Behind the scenes stuff
         *
         */

        struct key
        {
            bool currentlyHeld = false;
            bool stickyHeld = false;

            bool sticky()
            {
                bool isHeld = stickyHeld;
                stickyHeld = false;
                return isHeld;
            }
        };


        std::vector<KeyListener> *keyListeners = nullptr;
        key keys[400];


        void _keyHandler(GLFWwindow *, int key, int scancode, int action, int mods) //TODO: add support for non glfw windows
        {
            keys[key].currentlyHeld = action != GLFW_RELEASE;
            keys[key].stickyHeld    = action != GLFW_RELEASE;

            if(keyListeners!=nullptr)
            {
                for(auto listener : *keyListeners)
                {
                    listener((KeyboardKey::type)key,scancode,(KeyboardModifier::type)mods,(KeyboardKeyState::type)action);
                }
            }
        }

        /**
         *
         * interface
         *
         */

        bool keyDown(ge::KeyboardKey::type k)
        {
            keys[(int)k].stickyHeld = false;
            return keys[(int)k].currentlyHeld;
        }

        bool keyDownSticky(ge::KeyboardKey::type k)
        {
            return keys[(int)k].sticky() || keys[(int)k].currentlyHeld;
        }

        void insertKeyListener(KeyListener keyListener)
        {
            if(keyListeners == nullptr)
            {
                keyListeners = new std::vector<KeyListener>();
            }
            keyListeners->push_back(keyListener);
        }
    }

}
