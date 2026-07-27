#pragma once
#include "MeshFactory.h"

class Renderer;


class Scene
{
public:
    explicit Scene( Renderer &renderer );

    bool initialize();
    void shutdown();

private:
    Renderer &mRenderer;
    MeshFactory mMeshFactory;

    bool createLight();
    bool createObjects();
};
