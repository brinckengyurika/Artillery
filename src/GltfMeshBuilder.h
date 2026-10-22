#pragma once
#include <string>
#include <tiny_gltf.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <filesystem>
#include "ModelCache.h"

class Renderer;

namespace tinygltf {
class Model;
}

namespace Ogre {
class SceneManager;
class SceneNode;
}

class GltfMeshBuilder {
public:
//    explicit GltfMeshBuilder( Renderer &renderer );
    explicit GltfMeshBuilder(
    Renderer& renderer,
    ModelCache& modelCache);

    bool inspect( const tinygltf::Model &model );

    bool build(
        const tinygltf::Model &model,
        Ogre::SceneManager *sceneManager,
        Ogre::SceneNode *parentNode,
        const std::string &meshName,
        const std::string & assetKey
    );

    static std::string saveGltfImageAsPng(
        const tinygltf::Image& image,
        const std::filesystem::path& outputDir,
        const std::string& modelKey,
        int imageIndex
    );


private:
    Renderer& mRenderer;
    ModelCache& mModelCache;


    Ogre::SceneNode* buildNode(
        const tinygltf::Model &model,
        int nodeIndex,
        Ogre::SceneManager *sceneManager,
        Ogre::SceneNode *parentNode,
        const std::string &meshName,
        const std::string & assetKey
    );

    void buildMesh(
        const tinygltf::Model &model,
        int meshIndex,
        Ogre::SceneManager *sceneManager,
        Ogre::SceneNode *parentNode,
        const std::string &meshName,
        int nodeIndex,
        const std::string & assetKey
    );


};
