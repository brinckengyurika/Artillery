#pragma once

namespace Ogre
{
    class Item;
}

class Renderer;

class MeshFactory
{
public:
    explicit MeshFactory(Renderer &renderer);

    Ogre::Item* createCube();
    Ogre::Item* createPlane();
    Ogre::Item* createSphere();

private:
    Renderer &mRenderer;
};
