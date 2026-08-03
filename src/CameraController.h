#pragma once

#include <OgreCamera.h>

class InputManager;

class CameraController
{
public:
    CameraController();

    bool initialize(Ogre::Camera *camera);

    void update(const InputManager &input, float dt);

private:
    Ogre::Camera *mCamera{};

    float mMoveSpeed = 20.0f;
    float mMouseSensitivity = 0.0025f;
};
