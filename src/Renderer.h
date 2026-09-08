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

#ifdef None
#undef None
#endif
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlit.h>


#include "Terra/Hlms/OgreHlmsTerra.h"

namespace tinygltf {
class Model;
}

class Renderer {
public:
    Renderer();
    ~Renderer();
/*Refactoring
    bool initialize();
    bool renderFrame();
    void shutdown();
*/
    bool initialize();
    void update(float dt);
    bool renderFrame();
    void shutdown();

    Ogre::Root *getRoot() const {
        return mRoot;
    }

    Ogre::SceneManager *getSceneManager() const {
        return mSceneManager;
    }

    Ogre::Window *getWindow() const {
        return mWindow;
    }

    Ogre::Camera *getCamera() const {
        return mCamera;
    }

    Ogre::HlmsUnlit *getHlmsUnlit() const {
        return mHlmsUnlit;
    }

    Ogre::HlmsPbs *getHlmsPbs() const {
        return mHlmsPbs;
    }
    void setHlmsUnlit(Ogre::HlmsUnlit *hlms) {
        mHlmsUnlit = hlms;
    }

    void setHlmsPbs(Ogre::HlmsPbs *hlms) {
        mHlmsPbs = hlms;
    }


    Ogre::HlmsTerra *getHlmsTerra() const {
        return mHlmsTerra;
    }

    void setHlmsTerra(Ogre::HlmsTerra *hlms) {
        mHlmsTerra = hlms;
    }
/*
    Ogre::Terra *getTerra() const {
        return mTerra;
    }
*/
Ogre::HlmsDatablock *getTerraDatablock() const
{
    return mResources.getTerraDatablock();
}

private:
    Ogre::HlmsPbs   *mHlmsPbs = nullptr;
    Ogre::HlmsUnlit *mHlmsUnlit = nullptr;

    Ogre::HlmsTerra *mHlmsTerra = nullptr;

    Ogre::Root          *mRoot          = nullptr;
    Ogre::Window        *mWindow        = nullptr;
    Ogre::SceneManager  *mSceneManager  = nullptr;
    Ogre::Camera        *mCamera        = nullptr;

    ResourceManager mResources;
    Scene mScene;
    CameraController mCameraController;

    Ogre::CompositorWorkspace *mWorkspace = nullptr;

    GltfLoader mGltfLoader;
    GltfMeshBuilder mMeshBuilder;
    InputManager mInputManager;


//    Ogre::Terra *mTerra = nullptr;

    bool createLight();
    bool createTerrain();
    bool createObjects();


};
