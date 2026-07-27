
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "GltfLoader.h"

#include <iostream>

#include "tiny_gltf.h"

#include "Renderer.h"

GltfLoader::GltfLoader(Renderer &renderer) :
    mRenderer(renderer)
{
}

bool GltfLoader::load(const std::string &filename)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;

    bool ok = loader.LoadBinaryFromFile(
        &model,
        &err,
        &warn,
        filename
    );

    if(!warn.empty())
        std::cout << warn << std::endl;

    if(!err.empty())
        std::cerr << err << std::endl;

    if(!ok)
        return false;

    std::cout << "GLTF loaded successfully\n";

    std::cout << "Meshes      : " << model.meshes.size() << std::endl;
    std::cout << "Nodes       : " << model.nodes.size() << std::endl;
    std::cout << "Materials   : " << model.materials.size() << std::endl;
    std::cout << "Textures    : " << model.textures.size() << std::endl;
    std::cout << "Images      : " << model.images.size() << std::endl;
    std::cout << "Animations  : " << model.animations.size() << std::endl;

    return true;
}
