#pragma once

#include "Renderer.h"

class Engine
{
public:
    bool initialize();
    void run();
    void shutdown();

private:
    Renderer mRenderer;
};
