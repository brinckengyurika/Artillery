#pragma once
#include "MeshFactory.h"
#include "Terra/Terra.h"

class Renderer;


class Scene
{
public:
    explicit Scene( Renderer &renderer );
    bool initialize();
    void update(float dt);
    void shutdown();

private:
    Renderer &mRenderer;
    MeshFactory mMeshFactory;

    Ogre::Terra *mTerra = nullptr;

    bool createLight();
    bool createObjects();
};
