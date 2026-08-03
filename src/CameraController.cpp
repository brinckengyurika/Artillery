#include "CameraController.h"

#include "InputManager.h"

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <iostream>

CameraController::CameraController()
{
}

bool CameraController::initialize(Ogre::Camera *camera)
{
    mCamera = camera;
    return true;
}

void CameraController::update(const InputManager &input, float dt)
{
    Ogre::Vector3 move = Ogre::Vector3::ZERO;

    if( input.keyDown( XK_w ) )
        move += Ogre::Vector3::NEGATIVE_UNIT_Z;

    if( input.keyDown( XK_s ) )
        move += Ogre::Vector3::UNIT_Z;

    if( input.keyDown( XK_a ) )
        move += Ogre::Vector3::NEGATIVE_UNIT_X;

    if( input.keyDown( XK_d ) )
        move += Ogre::Vector3::UNIT_X;

    if( input.keyDown( XK_q ) )
        move += Ogre::Vector3::NEGATIVE_UNIT_Y;

    if( input.keyDown( XK_e ) )
        move += Ogre::Vector3::UNIT_Y;

    if( move != Ogre::Vector3::ZERO )
        move.normalise();

    float speed = mMoveSpeed;

    if( input.keyDown( XK_Shift_L ) ||
        input.keyDown( XK_Shift_R ) )
    {
        speed *= 4.0f;
    }

    mCamera->moveRelative( move * speed * dt );

    if( input.rightButtonDown() )
    {
        mCamera->yaw(
            Ogre::Radian(
                -input.mouseDeltaX() * mMouseSensitivity ) );

        mCamera->pitch(
            Ogre::Radian(
                -input.mouseDeltaY() * mMouseSensitivity ) );
    }
}
