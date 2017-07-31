//
// Created by Ethan Breit on 2017-07-29.
//

#include <OpenGL/factories/GLRenderObjectFactory.h>
#include <OpenGL/types/GLRenderObject.h>

namespace ge
{
    namespace GL
    {
        ge::RenderObject *RenderObjectFactory::newRenderObject()
        {
            return new GL::RenderObject();
        }
    }
}