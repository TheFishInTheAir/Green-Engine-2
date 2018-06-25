//
// Created by Ethan Breit on 2017-08-08.
//
#include <ge/debug/FreeMove.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/input/MouseHandler.h>
#include <ge/util/RotationUtil.h>
#include <ge/console/ConsoleIO.h>

namespace ge
{
    void FreeMove::update(ge::Camera *camera)
    {
        float localSpeed = speed;
        double x = 0;
        double y = 0;

        if(KeyboardHandler::keyDown(KeyboardKey::LSHIFT))
            localSpeed *= 2;
        if(KeyboardHandler::keyDown(KeyboardKey::TAB))
            localSpeed /= 2;

        MouseHandler::getMousePos(&x, &y);
        _horizontalAngle += float(-x)*lookSpeed;
        _verticalAngle  +=  float(-y)*lookSpeed;
        MouseHandler::resetMousePos();


        glm::vec3 direction = RotationUtil::getDirection(_verticalAngle,_horizontalAngle);

        glm::vec3 right = RotationUtil::getRight(_horizontalAngle);

        glm::vec3 up = glm::cross( right, direction );



        if(KeyboardHandler::keyDown(KeyboardKey::W))
        {
            _position += direction * localSpeed;
        }

        if(KeyboardHandler::keyDown(KeyboardKey::S))
            _position -= direction*localSpeed;
        if(KeyboardHandler::keyDown(KeyboardKey::A))
            _position -= right*localSpeed;
        if(KeyboardHandler::keyDown(KeyboardKey::D))
            _position += right*localSpeed;
        if(KeyboardHandler::keyDown(KeyboardKey::SPACE))
            _position += up*localSpeed;
        if(KeyboardHandler::keyDown(KeyboardKey::LCTRL))
            _position -= up*localSpeed;

        camera->pos = _position;
        camera->up = up;
        camera->dir = _position+direction;
    }
}
