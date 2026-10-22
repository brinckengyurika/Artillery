#pragma once
#include "Terra/Hlms/OgreHlmsTerra.h"
#include "ModelCache.h"
class Renderer;

class ResourceManager {
public:
    explicit ResourceManager(Renderer &renderer, ModelCache& mModelCache);

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
    ModelCache& mModelCache;
    bool setupResources();
    bool registerHlms();
    bool loadResources();
    Ogre::HlmsTerra *mHlmsTerra = nullptr;
    Ogre::HlmsDatablock *mTerraDatablock = nullptr;
};
