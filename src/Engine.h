#pragma once
#include "Config.h"
#include "Renderer.h"
#include "ModelCache.h"


class Engine {
public:
    Engine(const Config& config, ModelCache& mModelCache );
    bool initialize();
    void run();
    void shutdown();

private:
    Renderer mRenderer;
    const Config& config;
};
