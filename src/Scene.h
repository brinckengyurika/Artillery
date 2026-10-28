#pragma once
#include "MeshFactory.h"
#include "Terra/Terra.h"
#include "ModelCache.h"


class Renderer;


class Scene {
public:
    explicit Scene( Renderer &renderer );
    bool initialize();
//    bool createTerra();
    void update(float dt);
    void shutdown();




private:
    ModelCache modelCache;

    Renderer &mRenderer;
    MeshFactory mMeshFactory;

    Ogre::Terra *mTerra = nullptr;

    bool createLight();
    bool createObjects();
    bool createObjectsNew(int sceneindex);
    bool createTerra(int sceneindex);
};
