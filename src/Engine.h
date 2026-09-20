#pragma once
#include "Config.h"
#include "Renderer.h"


class Engine {
public:
    Engine(const Config& config );
    bool initialize();
    void run();
    void shutdown();

private:
    Renderer mRenderer;
    const Config& config;
};
