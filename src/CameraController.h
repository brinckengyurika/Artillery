#pragma once

namespace Ogre
{
    class Camera;
    class SceneNode;
}

class CameraController
{
public:
    CameraController();

    bool initialize(
        Ogre::Camera *camera,
        Ogre::SceneNode *node );

    void update( float dt );

private:

    Ogre::Camera *mCamera{};
    Ogre::SceneNode *mNode{};

    float mMoveSpeed = 10.0f;
    float mRotateSpeed = 0.2f;
};
