#include "Engine.h"

#include <iostream>


Engine::Engine(const Config& config ):
    config(config),
    mRenderer(config) {
    //mRenderer(config);
}

bool Engine::initialize() {
    std::cout << "Engine initialize()" << std::endl;

    return mRenderer.initialize();

}

void Engine::run() {
    std::cout << "Engine run()" << std::endl;

    while( mRenderer.renderFrame() ) {
    }
    /*Refactoring
        while( mRenderer.renderFrame() )
        {
        }
    */
}

void Engine::shutdown() {
    std::cout << "Engine shutdown()" << std::endl;
    mRenderer.shutdown();
}
