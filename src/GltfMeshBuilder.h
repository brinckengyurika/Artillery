#pragma once

#include <string>

#include <tiny_gltf.h>


namespace tinygltf{
    class Model;
}

class Renderer;

class GltfMeshBuilder{
public:
    explicit GltfMeshBuilder( Renderer &renderer );
    bool inspect( const tinygltf::Model &model );

    bool build( const tinygltf::Model &model, const std::string &meshName );

private:
    Renderer &mRenderer;
};
