#pragma once
#include "Terra/Hlms/OgreHlmsTerra.h"
class Renderer;

class ResourceManager {
public:
    explicit ResourceManager(Renderer &renderer);

    bool initialize();
    void shutdown();
    Ogre::HlmsTerra *getHlmsTerra() const {
        return mHlmsTerra;
    }

    Ogre::HlmsDatablock *getTerraDatablock() const {
        return mTerraDatablock;
    }
private:
    Renderer &mRenderer;
    bool setupResources();
    bool registerHlms();
    bool loadResources();
    Ogre::HlmsTerra *mHlmsTerra = nullptr;
    Ogre::HlmsDatablock *mTerraDatablock = nullptr;
};
