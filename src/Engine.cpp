#include "Engine.h"

#include <iostream>

bool Engine::initialize()
{
    std::cout << "Engine initialize()" << std::endl;

    return mRenderer.initialize();
}

void Engine::run()
{
    std::cout << "Engine run()" << std::endl;

    while( mRenderer.renderFrame() )
    {
    }
}

void Engine::shutdown()
{
    std::cout << "Engine shutdown()" << std::endl;


    mRenderer.shutdown();
}
