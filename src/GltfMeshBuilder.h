#pragma once

#include <string>

#include <tiny_gltf.h>


namespace tinygltf{
    class Model;
}

namespace Ogre
{
    class SceneManager;
    class SceneNode;
}


class Renderer;

class GltfMeshBuilder{
public:
    explicit GltfMeshBuilder( Renderer &renderer );
    bool inspect( const tinygltf::Model &model );

    bool build( const tinygltf::Model &model, Ogre::SceneManager *sceneManager, Ogre::SceneNode *parentNode, const std::string &meshName);

private:
    Renderer &mRenderer;
};
