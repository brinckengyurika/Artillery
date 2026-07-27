#pragma once

class Renderer;

class ResourceManager
{
public:
    explicit ResourceManager(Renderer &renderer);

    bool initialize();
    void shutdown();

private:
    Renderer &mRenderer;
    bool setupResources();
    bool registerHlms();
    bool loadResources();
};
