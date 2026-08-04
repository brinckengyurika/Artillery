#pragma once

#include <OgreRoot.h>
#include <OgreWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>

#include "ResourceManager.h"
#include "Scene.h"
#include "CameraController.h"

#include "GltfLoader.h"
#include "GltfMeshBuilder.h"
#include "InputManager.h"

namespace tinygltf
{
    class Model;
}

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool initialize();
    bool renderFrame();
    void shutdown();

    Ogre::Root *getRoot() const
    {
        return mRoot;
    }

    Ogre::SceneManager *getSceneManager() const
    {
        return mSceneManager;
    }

    Ogre::Window *getWindow() const
    {
        return mWindow;
    }

    Ogre::Camera *getCamera() const
    {
        return mCamera;
    }



private:

    Ogre::Root *mRoot{};
    Ogre::Window *mWindow{};
    Ogre::SceneManager *mSceneManager{};
    Ogre::Camera *mCamera{};


    ResourceManager mResources;
    Scene mScene;
    CameraController mCameraController;

    Ogre::CompositorWorkspace *mWorkspace{};
    GltfLoader mGltfLoader;
    GltfMeshBuilder mMeshBuilder;
    InputManager mInputManager;



};
