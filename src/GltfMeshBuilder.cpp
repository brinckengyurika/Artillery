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



bool GltfMeshBuilder::build(
    const tinygltf::Model &model,
    Ogre::SceneManager *sceneManager,
    Ogre::SceneNode *parentNode,
    const std::string &meshName) {
    std::cout
            << "Invoking build, parent name : "
            << parentNode->getName()
            << std::endl;

    std::cout
            << "Invoking build, meshName name : "
            << meshName
            << std::endl;

    if(!sceneManager || !parentNode)
        return false;

    if(model.meshes.empty()) {
        std::cerr
                << "GLTF contains no meshes."
                << std::endl;

        return false;
    }

    if(model.nodes.empty()) {
        std::cerr
                << "GLTF contains no nodes."
                << std::endl;

        return false;
    }

    //---------------------------------------------------------
    // Root node for the imported model
    //---------------------------------------------------------

    Ogre::SceneNode *mainNode =
        parentNode->createChildSceneNode();

    mainNode->setName(meshName);

    //---------------------------------------------------------
    // Find glTF scene
    //---------------------------------------------------------

    int sceneIndex = model.defaultScene;

    if(sceneIndex < 0 ||
            sceneIndex >= static_cast<int>(model.scenes.size())) {
        sceneIndex = 0;
    }

    if(model.scenes.empty()) {
        std::cerr
                << "GLTF contains no scenes."
                << std::endl;

        return false;
    }

    const tinygltf::Scene &scene =
        model.scenes[sceneIndex];

    std::cout
            << "GLTF scene: "
            << scene.name
            << std::endl;

    //---------------------------------------------------------
    // Build all root nodes
    //---------------------------------------------------------

    for(int nodeIndex : scene.nodes) {
        buildNode(
            model,
            nodeIndex,
            sceneManager,
            mainNode,
            meshName
        );
    }

    return true;
}




Ogre::SceneNode* GltfMeshBuilder::buildNode(
    const tinygltf::Model &model,
    int nodeIndex,
    Ogre::SceneManager *sceneManager,
    Ogre::SceneNode *parentNode,
    const std::string &meshName) {
    if(nodeIndex < 0 ||
            nodeIndex >= static_cast<int>(model.nodes.size())) {
        return nullptr;
    }

    const tinygltf::Node &gltfNode =
        model.nodes[nodeIndex];

    //---------------------------------------------------------
    // Create Ogre node
    //---------------------------------------------------------

    Ogre::SceneNode *node =
        parentNode->createChildSceneNode();

    std::string nodeName =
        gltfNode.name;

    if(nodeName.empty()) {
        nodeName =
            meshName
            + "_node_"
            + std::to_string(nodeIndex);
    }

    node->setName(nodeName);

    //---------------------------------------------------------
    // Translation
    //---------------------------------------------------------

    if(gltfNode.translation.size() == 3) {
        node->setPosition(
            static_cast<Ogre::Real>(gltfNode.translation[0]),
            static_cast<Ogre::Real>(gltfNode.translation[1]),
            static_cast<Ogre::Real>(gltfNode.translation[2])
        );
    }

    //---------------------------------------------------------
    // Rotation
    //
    // glTF quaternion order:
    // x, y, z, w
    //---------------------------------------------------------

    if(gltfNode.rotation.size() == 4) {
        Ogre::Quaternion q(
            static_cast<Ogre::Real>(gltfNode.rotation[3]),
            static_cast<Ogre::Real>(gltfNode.rotation[0]),
            static_cast<Ogre::Real>(gltfNode.rotation[1]),
            static_cast<Ogre::Real>(gltfNode.rotation[2])
        );

        node->setOrientation(q);
    }

    //---------------------------------------------------------
    // Scale
    //---------------------------------------------------------

    if(gltfNode.scale.size() == 3) {
        node->setScale(
            static_cast<Ogre::Real>(gltfNode.scale[0]),
            static_cast<Ogre::Real>(gltfNode.scale[1]),
            static_cast<Ogre::Real>(gltfNode.scale[2])
        );
    }

    //---------------------------------------------------------
    // Mesh
    //---------------------------------------------------------

    if(gltfNode.mesh >= 0) {
        buildMesh(
            model,
            gltfNode.mesh,
            sceneManager,
            node,
            meshName,
            nodeIndex
        );
    }

    //---------------------------------------------------------
    // Children
    //---------------------------------------------------------

    for(int childIndex : gltfNode.children) {
        buildNode(
            model,
            childIndex,
            sceneManager,
            node,
            meshName
        );
    }

    return node;
}


void GltfMeshBuilder::buildMesh(
    const tinygltf::Model &model,
    int meshIndex,
    Ogre::SceneManager *sceneManager,
    Ogre::SceneNode *parentNode,
    const std::string &meshName,
    int nodeIndex) {
    if(meshIndex < 0 ||
            meshIndex >= static_cast<int>(model.meshes.size())) {
        std::cerr
                << "Invalid mesh index: "
                << meshIndex
                << std::endl;

        return;
    }

    const tinygltf::Mesh &mesh =
        model.meshes[meshIndex];

    std::cout
            << "Build mesh "
            << meshIndex
            << ": "
            << mesh.name
            << std::endl;

    size_t primitiveIndex = 0;

    for(const tinygltf::Primitive &primitive :
            mesh.primitives) {
        std::cout
                << "  Primitive "
                << primitiveIndex
                << std::endl;

        //-----------------------------------------------------
        // Only triangles for now
        //-----------------------------------------------------

        if(primitive.mode != TINYGLTF_MODE_TRIANGLES) {
            std::cerr
                    << "  Unsupported primitive mode: "
                    << primitive.mode
                    << std::endl;

            ++primitiveIndex;
            continue;
        }

        //-----------------------------------------------------
        // POSITION
        //-----------------------------------------------------

        auto posIt =
            primitive.attributes.find("POSITION");

        if(posIt == primitive.attributes.end()) {
            std::cerr
                    << "  Primitive has no POSITION attribute."
                    << std::endl;

            ++primitiveIndex;
            continue;
        }

        const tinygltf::Accessor &posAccessor =
            model.accessors[posIt->second];

        const tinygltf::BufferView &posView =
            model.bufferViews[posAccessor.bufferView];

        const tinygltf::Buffer &posBuffer =
            model.buffers[posView.buffer];

        const unsigned char *posData =
            posBuffer.data.data()
            + posView.byteOffset
            + posAccessor.byteOffset;

        const size_t vertexCount =
            posAccessor.count;

        const size_t posStride =
            posView.byteStride != 0
            ? posView.byteStride
            : 3 * sizeof(float);

        std::vector<Ogre::Vector3> positions(
            vertexCount);

        for(size_t i = 0;
                i < vertexCount;
                ++i) {
            const float *p =
                reinterpret_cast<const float *>(
                    posData + i * posStride);

            positions[i] =
                Ogre::Vector3(
                    p[0],
                    p[1],
                    p[2]
                );
        }

        //-----------------------------------------------------
        // NORMAL
        //-----------------------------------------------------

        std::vector<Ogre::Vector3> normals;

        auto normalIt =
            primitive.attributes.find("NORMAL");

        if(normalIt != primitive.attributes.end()) {
            const tinygltf::Accessor &accessor =
                model.accessors[normalIt->second];

            const tinygltf::BufferView &view =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer &buffer =
                model.buffers[view.buffer];

            const unsigned char *data =
                buffer.data.data()
                + view.byteOffset
                + accessor.byteOffset;

            const size_t stride =
                view.byteStride != 0
                ? view.byteStride
                : 3 * sizeof(float);

            normals.resize(accessor.count);

            for(size_t i = 0;
                    i < accessor.count;
                    ++i) {
                const float *n =
                    reinterpret_cast<const float *>(
                        data + i * stride);

                normals[i] =
                    Ogre::Vector3(
                        n[0],
                        n[1],
                        n[2]
                    );
            }
        }

        //-----------------------------------------------------
        // TEXCOORD_0
        //-----------------------------------------------------

        std::vector<Ogre::Vector2> uvs;

        auto uvIt =
            primitive.attributes.find("TEXCOORD_0");

        if(uvIt != primitive.attributes.end()) {
            const tinygltf::Accessor &accessor =
                model.accessors[uvIt->second];

            const tinygltf::BufferView &view =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer &buffer =
                model.buffers[view.buffer];

            const unsigned char *data =
                buffer.data.data()
                + view.byteOffset
                + accessor.byteOffset;

            const size_t stride =
                view.byteStride != 0
                ? view.byteStride
                : 2 * sizeof(float);

            uvs.resize(accessor.count);

            for(size_t i = 0;
                    i < accessor.count;
                    ++i) {
                const float *uv =
                    reinterpret_cast<const float *>(
                        data + i * stride);

                uvs[i] =
                    Ogre::Vector2(
                        uv[0],
                        uv[1]
                    );
            }
        }

        //-----------------------------------------------------
        // INDICES
        //-----------------------------------------------------

        if(primitive.indices < 0) {
            std::cerr
                    << "  Primitive has no indices."
                    << std::endl;

            ++primitiveIndex;
            continue;
        }

        const tinygltf::Accessor &indexAccessor =
            model.accessors[primitive.indices];

        const tinygltf::BufferView &indexView =
            model.bufferViews[indexAccessor.bufferView];

        const tinygltf::Buffer &indexBuffer =
            model.buffers[indexView.buffer];

        const unsigned char *indexData =
            indexBuffer.data.data()
            + indexView.byteOffset
            + indexAccessor.byteOffset;

        std::vector<unsigned int> indices(
            indexAccessor.count);

        for(size_t i = 0;
                i < indexAccessor.count;
                ++i) {
            switch(indexAccessor.componentType) {
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:

                indices[i] =
                    reinterpret_cast<
                    const unsigned char *>(
                        indexData)[i];

                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:

                indices[i] =
                    reinterpret_cast<
                    const unsigned short *>(
                        indexData)[i];

                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:

                indices[i] =
                    reinterpret_cast<
                    const unsigned int *>(
                        indexData)[i];

                break;

            default:

                std::cerr
                        << "Unsupported index component type: "
                        << indexAccessor.componentType
                        << std::endl;

                indices.clear();

                break;
            }

            if(indices.empty())
                break;
        }

        if(indices.empty()) {
            ++primitiveIndex;
            continue;
        }

        //-----------------------------------------------------
        // ManualObject
        //-----------------------------------------------------

        std::string objectName =
            meshName
            + "_mesh_"
            + std::to_string(meshIndex)
            + "_node_"
            + std::to_string(nodeIndex)
            + "_primitive_"
            + std::to_string(primitiveIndex);

        Ogre::ManualObject *manual =
            sceneManager->createManualObject();

        manual->setName(objectName);

        manual->begin(
            "GltfDefault",
            Ogre::OT_TRIANGLE_LIST
        );

        //-----------------------------------------------------
        // Vertices
        //-----------------------------------------------------

        for(size_t i = 0; i < positions.size(); ++i) {
            manual->position(positions[i]);

            if(i < normals.size())
                manual->normal(normals[i]);

            if(i < uvs.size())
                manual->textureCoord(uvs[i]);
        }

        for(unsigned int index : indices)
            manual->index(index);

        manual->end();
        //-----------------------------------------------------
        // Indices
        //-----------------------------------------------------

        for(unsigned int index : indices) {
            manual->index(index);
        }

        manual->end();

        //-----------------------------------------------------
        // Attach
        //-----------------------------------------------------

        parentNode->attachObject(manual);

        std::cout
                << "  Created Ogre object: "
                << objectName
                << std::endl;

        ++primitiveIndex;
    }
}
