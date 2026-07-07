#include "Engine.h"

#include <iostream>

Engine::Engine()
{
}

Engine::~Engine()
{
    delete mRoot;
}

bool Engine::initialize()
{
    std::cout << "Creating Ogre Root..." << std::endl;

    mRoot = new Ogre::Root(
        "cfg/plugins.cfg",
        "",
        "Ogre.log"
    );

    const Ogre::RenderSystemList &renderers = mRoot->getAvailableRenderers();

    if( renderers.empty() )
    {
        std::cerr << "No RenderSystems found!" << std::endl;
        return false;
    }

    mRoot->setRenderSystem( renderers.front() );

    mWindow = mRoot->initialise(
        true,
        "Artillery"
    );

    return true;
}


void Engine::run()
{
    while( !mWindow->isClosed() )
    {
        mRoot->renderOneFrame();
    }
}

void Engine::shutdown()
{
    std::cout << "Engine shutdown()" << std::endl;
}
