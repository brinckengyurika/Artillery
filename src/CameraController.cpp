#include "CameraController.h"

#include <OgreCamera.h>
#include <OgreSceneNode.h>

CameraController::CameraController()
{
}

bool CameraController::initialize(
    Ogre::Camera *camera,
    Ogre::SceneNode *node )
{
    mCamera = camera;
    mNode = node;

    return true;
}

void CameraController::update( float dt )
{
    // később ide jön
}
