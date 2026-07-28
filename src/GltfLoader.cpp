#include "GltfLoader.h"

#include <tiny_gltf.h>

#include <iostream>

#include "Renderer.h"

GltfLoader::GltfLoader( Renderer &renderer ) :
    mRenderer( renderer )
{
}

bool GltfLoader::load( const std::string &filename )
{
    tinygltf::Model model;

    if( !load( filename, model ) )
        return false;

    return true;
}

bool GltfLoader::load( const std::string &filename,
                       tinygltf::Model &model )
{
    tinygltf::TinyGLTF loader;

    std::string err;
    std::string warn;

    bool ok = loader.LoadBinaryFromFile(
        &model,
        &err,
        &warn,
        filename
    );

    if( !warn.empty() )
        std::cout << "TinyGLTF warning: "
                  << warn
                  << std::endl;

    if( !err.empty() )
        std::cerr << "TinyGLTF error: "
                  << err
                  << std::endl;

    if( !ok )
        return false;

    std::cout << "Loaded GLB: "
              << filename
              << std::endl;

    return true;
}
