#include "GltfMeshBuilder.h"

#include <tiny_gltf.h>

#include "Renderer.h"

#include <iostream>

#include <OgreManualObject2.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>

GltfMeshBuilder::GltfMeshBuilder( Renderer &renderer ) :
    mRenderer( renderer ) {
}

bool GltfMeshBuilder::inspect( const tinygltf::Model &model ) {
    std::cout << std::endl;
    std::cout << "===== glTF Model =====" << std::endl;

    std::cout << "Scenes      : " << model.scenes.size() << std::endl;
    std::cout << "Nodes       : " << model.nodes.size() << std::endl;
    std::cout << "Meshes      : " << model.meshes.size() << std::endl;
    std::cout << "Materials   : " << model.materials.size() << std::endl;
    std::cout << "Images      : " << model.images.size() << std::endl;
    std::cout << "Textures    : " << model.textures.size() << std::endl;
    std::cout << "Accessors   : " << model.accessors.size() << std::endl;
    std::cout << "BufferViews : " << model.bufferViews.size() << std::endl;
    std::cout << "Buffers     : " << model.buffers.size() << std::endl;

    std::cout << "\nMeshes:\n";

    for( size_t i = 0; i < model.meshes.size(); ++i ) {
        const tinygltf::Mesh &mesh = model.meshes[i];

        std::cout
                << "Mesh "
                << i
                << ": "
                << mesh.name
                << std::endl;

        std::cout
                << "  Primitives: "
                << mesh.primitives.size()
                << std::endl;
        for (size_t p = 0 ; p < mesh.primitives.size(); ++p) {
            ;
            auto it = mesh.primitives.at(p).attributes.find( "POSITION" );

            if( it == mesh.primitives.at(p).attributes.end() ) {
                std::cout << "No POSITION attribute!" << std::endl;
                return false;
            }

            int accessorIndex = it->second;
            const tinygltf::Accessor &accessor =
                model.accessors[accessorIndex];
            std::cout << "\t\tPOSITION accessor" << std::endl;
            std::cout << "\t\tcount = " << accessor.count << std::endl;
            std::cout << "\t\ttype = " << accessor.type << std::endl;
            std::cout << "\t\tcomponentType = " << accessor.componentType << std::endl;

            if( accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
                    accessor.type != TINYGLTF_TYPE_VEC3 ) {
                std::cout << "Unsupported POSITION format!" << std::endl;
                return false;
            }

            const tinygltf::BufferView &bufferView =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer &buffer =
                model.buffers[bufferView.buffer];

            const unsigned char *data =
                buffer.data.data() +
                bufferView.byteOffset +
                accessor.byteOffset;

            const float *vertices =
                reinterpret_cast<const float *>( data );
            std::cout << "\nFirst vertices:\n";

            size_t count = std::min<size_t>( accessor.count, 10 );

            for( size_t i = 0; i < count; ++i ) {
                float x = vertices[i * 3 + 0];
                float y = vertices[i * 3 + 1];
                float z = vertices[i * 3 + 2];

                std::cout
                        << i
                        << ": "
                        << x << ", "
                        << y << ", "
                        << z
                        << std::endl;
            }

            const tinygltf::Accessor &indexAccessor =
                model.accessors[ mesh.primitives.at(p).indices ];
            const tinygltf::BufferView &indexBufferView =
                model.bufferViews[ indexAccessor.bufferView ];

            const tinygltf::Buffer &indexBuffer =
                model.buffers[ indexBufferView.buffer ];

            const unsigned char *indexData =
                indexBuffer.data.data() +
                indexBufferView.byteOffset +
                indexAccessor.byteOffset;

            std::cout
                    << "Index component type: "
                    << indexAccessor.componentType
                    << std::endl;

            std::cout
                    << "Index count: "
                    << indexAccessor.count
                    << std::endl;


        }

    }
    return true;
}
bool GltfMeshBuilder::build( const tinygltf::Model &model,
                             const std::string &meshName ) {
    std::cout << "Build this mesh: " << meshName << std::endl;
    if( model.meshes.empty() )
        return false;

    const tinygltf::Mesh &mesh = model.meshes.front();

    if( mesh.primitives.empty() )
        return false;

    const tinygltf::Primitive &primitive =
        mesh.primitives.front();

    //-----------------------------------------------------
    // POSITION
    //-----------------------------------------------------

    auto posIt = primitive.attributes.find( "POSITION" );

    if( posIt == primitive.attributes.end() )
        return false;

    const tinygltf::Accessor &posAccessor =
        model.accessors[posIt->second];

    const tinygltf::BufferView &posView =
        model.bufferViews[posAccessor.bufferView];


    const tinygltf::Buffer &posBuffer =
        model.buffers[posView.buffer];

    const unsigned char *posData =
        posBuffer.data.data() +
        posView.byteOffset +
        posAccessor.byteOffset;

    const float *positions =
        reinterpret_cast<const float *>( posData );
std::cout << "Meshes: "
          << model.meshes.size()
          << std::endl;

std::cout << "Primitives: "
          << mesh.primitives.size()
          << std::endl;

std::cout << "Primitive mode = "
          << primitive.mode
          << std::endl;

std::cout << "POSITION stride = "
          << posView.byteStride
          << std::endl;

    //-----------------------------------------------------
    // NORMAL
    //-----------------------------------------------------

    const float *normals = nullptr;

    auto normalIt = primitive.attributes.find( "NORMAL" );

    if( normalIt != primitive.attributes.end() ) {
        const tinygltf::Accessor &normalAccessor =
            model.accessors[normalIt->second];

        const tinygltf::BufferView &normalView =
            model.bufferViews[normalAccessor.bufferView];

        const tinygltf::Buffer &normalBuffer =
            model.buffers[normalView.buffer];

        normals =
            reinterpret_cast<const float *>(
                normalBuffer.data.data() +
                normalView.byteOffset +
                normalAccessor.byteOffset );
std::cout << "NORMAL stride = "
          << normalView.byteStride
          << std::endl;

    }
    //-----------------------------------------------------
    // TEXCOORD
    //-----------------------------------------------------


    const float *uvs = nullptr;

    auto uvIt = primitive.attributes.find( "TEXCOORD_0" );

    if( uvIt != primitive.attributes.end() ) {
        const tinygltf::Accessor &uvAccessor =
            model.accessors[uvIt->second];

        const tinygltf::BufferView &uvView =
            model.bufferViews[uvAccessor.bufferView];
std::cout << "UV stride = "
          << uvView.byteStride
          << std::endl;

        const tinygltf::Buffer &uvBuffer =
            model.buffers[uvView.buffer];

        uvs =
            reinterpret_cast<const float *>(
                uvBuffer.data.data() +
                uvView.byteOffset +
                uvAccessor.byteOffset );
    }
    //-----------------------------------------------------
    // INDICES
    //-----------------------------------------------------

    const tinygltf::Accessor &indexAccessor =
        model.accessors[primitive.indices];

    const tinygltf::BufferView &indexView =
        model.bufferViews[indexAccessor.bufferView];

    const tinygltf::Buffer &indexBuffer =
        model.buffers[indexView.buffer];

    const unsigned char *indexData =
        indexBuffer.data.data() +
        indexView.byteOffset +
        indexAccessor.byteOffset;
    //-----------------------------------------------------
    // ManualObject
    //-----------------------------------------------------



std::cout << "Index type = "
          << indexAccessor.componentType
          << std::endl;

std::cout << "Vertex count = "
          << posAccessor.count
          << std::endl;


std::cout << "POSITION accessor.byteOffset = "
          << posAccessor.byteOffset << std::endl;

std::cout << "POSITION bufferView.byteOffset = "
          << posView.byteOffset << std::endl;

std::cout << "POSITION bufferView.byteLength = "
          << posView.byteLength << std::endl;

std::cout << "POSITION accessor.count = "
          << posAccessor.count << std::endl;


std::cout << "INDEX accessor.byteOffset = "
          << indexAccessor.byteOffset << std::endl;

std::cout << "INDEX bufferView.byteOffset = "
          << indexView.byteOffset << std::endl;

std::cout << "INDEX count = "
          << indexAccessor.count << std::endl;

    Ogre::ManualObject *manual =
        mRenderer.getSceneManager()->createManualObject();

    manual->begin(
        "BaseWhite",
        Ogre::OT_TRIANGLE_LIST );

    //-----------------------------------------------------
    // Vertices
    //-----------------------------------------------------

    for( size_t i = 0; i < posAccessor.count; ++i ) {
        manual->position(
            positions[i * 3 + 0],
            positions[i * 3 + 1],
            positions[i * 3 + 2] );

        if( normals ) {
            manual->normal(
                normals[i * 3 + 0],
                normals[i * 3 + 1],
                normals[i * 3 + 2] );
        }

        if( uvs ) {
            manual->textureCoord(
                uvs[i * 2 + 0],
                uvs[i * 2 + 1] );
        }
    }

    //-----------------------------------------------------
    // Indices
    //-----------------------------------------------------

    switch( indexAccessor.componentType ) {
    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
        const uint16_t *indices =
            reinterpret_cast<const uint16_t *>( indexData );

        for( size_t i = 0; i < indexAccessor.count; ++i )
            manual->index( indices[i] );

        break;
    }

    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
        const uint32_t *indices =
            reinterpret_cast<const uint32_t *>( indexData );

        for( size_t i = 0; i < indexAccessor.count; ++i )
            manual->index( indices[i] );

        break;
    }

    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
        const uint8_t *indices =
            reinterpret_cast<const uint8_t *>( indexData );

        for( size_t i = 0; i < indexAccessor.count; ++i )
            manual->index( indices[i] );

        break;
    }

    default:
        manual->end();
        mRenderer.getSceneManager()->destroyManualObject( manual );
        return false;
    }

    //-----------------------------------------------------
    // Convert
    //-----------------------------------------------------

    manual->end();

    manual->convertToMesh( meshName );

    mRenderer.getSceneManager()->destroyManualObject( manual );
    std::cout << "This mesh ha sbeen builded : " << meshName << std::endl;
    return true;
}
