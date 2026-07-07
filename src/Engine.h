#pragma once

#include <OgreRoot.h>
#include <OgreWindow.h>

class Engine
{
public:
    Engine();
    ~Engine();

    bool initialize();
    void run();
    void shutdown();

private:
    Ogre::Root *mRoot = nullptr;
    Ogre::Window *mWindow = nullptr;
};
