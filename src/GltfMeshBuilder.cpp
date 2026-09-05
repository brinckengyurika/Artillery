#include "GltfMeshBuilder.h"

#include <tiny_gltf.h>

#include "Renderer.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <OgreManualObject2.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreMatrix3.h>
#include <OgreHlmsUnlit.h>
#include <OgreHlmsUnlitDatablock.h>

#include <OgreHlmsPbs.h>
#include <OgreHlmsPbsDatablock.h>


GltfMeshBuilder::GltfMeshBuilder (Renderer & renderer):
    mRenderer (renderer) {
}


bool
GltfMeshBuilder::inspect (const tinygltf::Model & model) {

    std::cout << std::endl;
    std::cout << "===== glTF Model =====" << std::endl;

    std::cout << "Scenes      : " << model.scenes.size () << std::endl;
    std::cout << "Nodes       : " << model.nodes.size () << std::endl;
    std::cout << "Meshes      : " << model.meshes.size () << std::endl;
    std::cout << "Materials   : " << model.materials.size () << std::endl;
    std::cout << "Images      : " << model.images.size () << std::endl;
    std::cout << "Textures    : " << model.textures.size () << std::endl;
    std::cout << "Accessors   : " << model.accessors.size () << std::endl;
    std::cout << "BufferViews : " << model.bufferViews.size () << std::endl;
    std::cout << "Buffers     : " << model.buffers.size () << std::endl;

    std::cout << "\nMeshes:\n";

    for (size_t i = 0; i < model.meshes.size (); ++i) {

        const tinygltf::Mesh & mesh = model.meshes[i];

        std::cout
                << "Mesh "
                << i
                << ": "
                << mesh.name
                << std::endl;

        std::cout
                << "  Primitives: "
                << mesh.primitives.size ()
                << std::endl;

        for (size_t p = 0; p < mesh.primitives.size (); ++p) {

            auto it =
                mesh.primitives.at (p).attributes.find ("POSITION");

            if (it == mesh.primitives.at (p).attributes.end ()) {

                std::cout
                        << "No POSITION attribute!"
                        << std::endl;

                return false;
            }

            int accessorIndex = it->second;

            const tinygltf::Accessor & accessor =
                model.accessors[accessorIndex];

            std::cout
                    << "\t\tPOSITION accessor"
                    << std::endl;

            std::cout
                    << "\t\tcount = "
                    << accessor.count
                    << std::endl;

            std::cout
                    << "\t\ttype = "
                    << accessor.type
                    << std::endl;

            std::cout
                    << "\t\tcomponentType = "
                    << accessor.componentType
                    << std::endl;

            if (accessor.componentType !=
                    TINYGLTF_COMPONENT_TYPE_FLOAT ||
                    accessor.type != TINYGLTF_TYPE_VEC3) {

                std::cout
                        << "Unsupported POSITION format!"
                        << std::endl;

                return false;
            }

            const tinygltf::BufferView & bufferView =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer & buffer =
                model.buffers[bufferView.buffer];

            const unsigned char *data =
                buffer.data.data () +
                bufferView.byteOffset +
                accessor.byteOffset;

            const float *vertices =
                reinterpret_cast < const float * >(data);

            std::cout
                    << "\nFirst vertices:\n";

            size_t count =
                std::min < size_t >(accessor.count, 10);

            for (size_t i = 0; i < count; ++i) {

                float x = vertices[i * 3 + 0];
                float y = vertices[i * 3 + 1];
                float z = vertices[i * 3 + 2];

                std::cout
                        << i
                        << ": "
                        << x
                        << ", "
                        << y
                        << ", "
                        << z
                        << std::endl;
            }

            const tinygltf::Accessor & indexAccessor =
                model.accessors[
                    mesh.primitives.at (p).indices
            ];

            const tinygltf::BufferView & indexBufferView =
                model.bufferViews[indexAccessor.bufferView];

            const tinygltf::Buffer & indexBuffer =
                model.buffers[indexBufferView.buffer];

            const unsigned char *indexData =
                indexBuffer.data.data () +
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


bool
GltfMeshBuilder::build (const tinygltf::Model & model,
                        Ogre::SceneManager *sceneManager,
                        Ogre::SceneNode *parentNode,
                        const std::string & meshName) {

    std::cout
            << "Invoking build, parent name : "
            << parentNode->getName ()
            << std::endl;

    std::cout
            << "Invoking build, meshName name : "
            << meshName
            << std::endl;

    if (!sceneManager || !parentNode)
        return false;

    if (model.meshes.empty ()) {

        std::cerr
                << "GLTF contains no meshes."
                << std::endl;

        return false;
    }

    if (model.nodes.empty ()) {

        std::cerr
                << "GLTF contains no nodes."
                << std::endl;

        return false;
    }

    //---------------------------------------------------------
    // Root node for the imported model
    //---------------------------------------------------------

    Ogre::SceneNode * mainNode =
        parentNode->createChildSceneNode ();

    mainNode->setName (meshName);

    //---------------------------------------------------------
    // Find glTF scene
    //---------------------------------------------------------

    int sceneIndex = model.defaultScene;

    if (sceneIndex < 0 ||
            sceneIndex >=
            static_cast < int >(model.scenes.size ())) {

        sceneIndex = 0;
    }

    if (model.scenes.empty ()) {

        std::cerr
                << "GLTF contains no scenes."
                << std::endl;

        return false;
    }

    const tinygltf::Scene & scene =
        model.scenes[sceneIndex];

    std::cout
            << "GLTF scene: "
            << scene.name
            << std::endl;

    //---------------------------------------------------------
    // Build all root nodes
    //---------------------------------------------------------

    std::cout
            << "Scene root nodes:"
            << std::endl;

    for (int nodeIndex : scene.nodes) {

        const tinygltf::Node & n =
            model.nodes[nodeIndex];

        std::cout
                << "  root node "
                << nodeIndex
                << " name='"
                << n.name
                << "' mesh="
                << n.mesh
                << " children="
                << n.children.size ()
                << std::endl;
    }

    for (int nodeIndex : scene.nodes) {

        buildNode (model,
                   nodeIndex,
                   sceneManager,
                   mainNode,
                   meshName);
    }

    return true;
}


Ogre::SceneNode *
GltfMeshBuilder::buildNode (const tinygltf::Model & model,
                            int nodeIndex,
                            Ogre::SceneManager *sceneManager,
                            Ogre::SceneNode *parentNode,
                            const std::string & meshName) {

    if (nodeIndex < 0 ||
            nodeIndex >=
            static_cast < int >(model.nodes.size ())) {

        return nullptr;
    }

    const tinygltf::Node & gltfNode =
        model.nodes[nodeIndex];

    std::cout
            << "NODE "
            << nodeIndex
            << " name='"
            << gltfNode.name
            << "' mesh="
            << gltfNode.mesh
            << " children="
            << gltfNode.children.size ()
            << " matrix="
            << gltfNode.matrix.size ()
            << " translation="
            << gltfNode.translation.size ()
            << " rotation="
            << gltfNode.rotation.size ()
            << " scale="
            << gltfNode.scale.size ()
            << std::endl;

    if (!gltfNode.matrix.empty ()) {

        std::cout
                << "NODE "
                << nodeIndex
                << " name='"
                << gltfNode.name
                << "' USES MATRIX"
                << std::endl;
    }

    std::cout
            << "NODE "
            << nodeIndex
            << " name='"
            << gltfNode.name
            << "' mesh="
            << gltfNode.mesh
            << " children="
            << gltfNode.children.size ()
            << std::endl;

    //---------------------------------------------------------
    // Create Ogre node
    //---------------------------------------------------------

    Ogre::SceneNode * node =
        parentNode->createChildSceneNode ();

    std::string nodeName =
        gltfNode.name;

    if (nodeName.empty ()) {

        nodeName =
            meshName +
            "_node_" +
            std::to_string (nodeIndex);
    }

    node->setName (nodeName);

    //---------------------------------------------------------
    // Node transformation
    //---------------------------------------------------------

    if (gltfNode.matrix.size () == 16) {

        const double *m =
            gltfNode.matrix.data ();

        Ogre::Matrix4 transform (
            static_cast < Ogre::Real >(m[0]),
            static_cast < Ogre::Real >(m[4]),
            static_cast < Ogre::Real >(m[8]),
            static_cast < Ogre::Real >(m[12]),

            static_cast < Ogre::Real >(m[1]),
            static_cast < Ogre::Real >(m[5]),
            static_cast < Ogre::Real >(m[9]),
            static_cast < Ogre::Real >(m[13]),

            static_cast < Ogre::Real >(m[2]),
            static_cast < Ogre::Real >(m[6]),
            static_cast < Ogre::Real >(m[10]),
            static_cast < Ogre::Real >(m[14]),

            static_cast < Ogre::Real >(m[3]),
            static_cast < Ogre::Real >(m[7]),
            static_cast < Ogre::Real >(m[11]),
            static_cast < Ogre::Real >(m[15])
        );

        Ogre::Vector3 position;
        Ogre::Vector3 scale;
        Ogre::Quaternion orientation;

        transform.decomposition (
            position,
            scale,
            orientation
        );

        node->setPosition (position);
        node->setScale (scale);
        node->setOrientation (orientation);

        std::cout
                << "  MATRIX transform:"
                << " pos=("
                << position.x << ", "
                << position.y << ", "
                << position.z
                << ")"
                << " scale=("
                << scale.x << ", "
                << scale.y << ", "
                << scale.z
                << ")"
                << std::endl;
    } else {

        if (gltfNode.translation.size () == 3) {

            node->setPosition (
                static_cast < Ogre::Real >(
                    gltfNode.translation[0]),

                static_cast < Ogre::Real >(
                    gltfNode.translation[1]),

                static_cast < Ogre::Real >(
                    gltfNode.translation[2])
            );
        }

        if (gltfNode.rotation.size () == 4) {

            Ogre::Quaternion q (
                static_cast < Ogre::Real >(
                    gltfNode.rotation[3]),

                static_cast < Ogre::Real >(
                    gltfNode.rotation[0]),

                static_cast < Ogre::Real >(
                    gltfNode.rotation[1]),

                static_cast < Ogre::Real >(
                    gltfNode.rotation[2])
            );

            node->setOrientation (q);
        }

        if (gltfNode.scale.size () == 3) {

            node->setScale (
                static_cast < Ogre::Real >(
                    gltfNode.scale[0]),

                static_cast < Ogre::Real >(
                    gltfNode.scale[1]),

                static_cast < Ogre::Real >(
                    gltfNode.scale[2])
            );
        }
    }

    //---------------------------------------------------------
    // Mesh
    //---------------------------------------------------------

    if (gltfNode.mesh >= 0) {

        std::cout
                << "  ATTACH MESH "
                << gltfNode.mesh
                << " TO NODE "
                << nodeIndex
                << " ('"
                << node->getName ()
                << "')"
                << std::endl;

        buildMesh (
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

    for (int childIndex : gltfNode.children) {

        buildNode (
            model,
            childIndex,
            sceneManager,
            node,
            meshName
        );
    }

    return node;
}


void
GltfMeshBuilder::buildMesh (
    const tinygltf::Model & model,
    int meshIndex,
    Ogre::SceneManager *sceneManager,
    Ogre::SceneNode *parentNode,
    const std::string & meshName,
    int nodeIndex) {

    if (meshIndex < 0 ||
            meshIndex >=
            static_cast < int >(model.meshes.size ())) {

        std::cerr
                << "Invalid mesh index: "
                << meshIndex
                << std::endl;

        return;
    }

    const tinygltf::Mesh & mesh =
        model.meshes[meshIndex];

    std::cout
            << "Build mesh "
            << meshIndex
            << ": "
            << mesh.name
            << " primitives="
            << mesh.primitives.size ()
            << std::endl;

    std::cout
            << "Build mesh "
            << meshIndex
            << ": "
            << mesh.name
            << std::endl;

    size_t primitiveIndex = 0;

    for (const tinygltf::Primitive & primitive :
            mesh.primitives) {

        std::cout
                << "  Primitive "
                << primitiveIndex
                << ": "
                << "material="
                << primitive.material
                << " mode="
                << primitive.mode
                << " indices="
                << primitive.indices
                << std::endl;

        std::cout
                << "    Attributes:"
                << std::endl;

        for (const auto & attribute :
                primitive.attributes) {

            std::cout
                    << "      "
                    << attribute.first
                    << " -> accessor "
                    << attribute.second
                    << std::endl;
        }

        std::cout
                << "  Primitive "
                << primitiveIndex
                << " mode="
                << primitive.mode
                << " material="
                << primitive.material
                << " indices="
                << primitive.indices
                << " attributes="
                << primitive.attributes.size ()
                << std::endl;

        for (const auto & attr :
                primitive.attributes) {

            std::cout
                    << "      "
                    << attr.first
                    << " -> accessor "
                    << attr.second
                    << std::endl;
        }

        //-----------------------------------------------------
        // Only triangles for now
        //-----------------------------------------------------

        if (primitive.mode != TINYGLTF_MODE_TRIANGLES) {

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
            primitive.attributes.find ("POSITION");

        if (posIt ==
                primitive.attributes.end ()) {

            std::cerr
                    << "  Primitive has no POSITION attribute."
                    << std::endl;

            ++primitiveIndex;
            continue;
        }

        const tinygltf::Accessor & posAccessor =
            model.accessors[posIt->second];

        const tinygltf::BufferView & posView =
            model.bufferViews[posAccessor.bufferView];

        const tinygltf::Buffer & posBuffer =
            model.buffers[posView.buffer];

        const unsigned char *posData =
            posBuffer.data.data () +
            posView.byteOffset +
            posAccessor.byteOffset;

        const size_t vertexCount =
            posAccessor.count;

        const size_t posStride =
            posView.byteStride != 0 ?
            posView.byteStride :
            3 * sizeof (float);

        std::vector < Ogre::Vector3 > positions (
            vertexCount
        );

        for (size_t i = 0;
                i < vertexCount;
                ++i) {

            const float *p =
                reinterpret_cast < const float * >(
                    posData + i * posStride
                );

            positions[i] =
                Ogre::Vector3 (
                    p[0],
                    p[1],
                    p[2]
                );
        }

        //-----------------------------------------------------
        // NORMAL
        //-----------------------------------------------------

        std::vector < Ogre::Vector3 > normals;

        auto normalIt =
            primitive.attributes.find ("NORMAL");

        if (normalIt !=
                primitive.attributes.end ()) {

            const tinygltf::Accessor & accessor =
                model.accessors[normalIt->second];

            const tinygltf::BufferView & view =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer & buffer =
                model.buffers[view.buffer];

            const unsigned char *data =
                buffer.data.data () +
                view.byteOffset +
                accessor.byteOffset;

            const size_t stride =
                view.byteStride != 0 ?
                view.byteStride :
                3 * sizeof (float);

            normals.resize (accessor.count);

            for (size_t i = 0;
                    i < accessor.count;
                    ++i) {

                const float *n =
                    reinterpret_cast < const float * >(
                        data + i * stride
                    );

                normals[i] =
                    Ogre::Vector3 (
                        n[0],
                        n[1],
                        n[2]
                    );
            }
        }

        //-----------------------------------------------------
        // TEXCOORD_0
        //-----------------------------------------------------

        std::vector < Ogre::Vector2 > uvs;

        auto uvIt =
            primitive.attributes.find ("TEXCOORD_0");

        if (uvIt !=
                primitive.attributes.end ()) {

            const tinygltf::Accessor & accessor =
                model.accessors[uvIt->second];

            const tinygltf::BufferView & view =
                model.bufferViews[accessor.bufferView];

            const tinygltf::Buffer & buffer =
                model.buffers[view.buffer];

            const unsigned char *data =
                buffer.data.data () +
                view.byteOffset +
                accessor.byteOffset;

            const size_t stride =
                view.byteStride != 0 ?
                view.byteStride :
                2 * sizeof (float);

            uvs.resize (accessor.count);

            for (size_t i = 0;
                    i < accessor.count;
                    ++i) {

                const float *uv =
                    reinterpret_cast < const float * >(
                        data + i * stride
                    );

                uvs[i] =
                    Ogre::Vector2 (
                        uv[0],
                        uv[1]
                    );
            }
        }

        //-----------------------------------------------------
        // INDICES
        //-----------------------------------------------------

        if (primitive.indices < 0) {

            std::cerr
                    << "  Primitive has no indices."
                    << std::endl;

            ++primitiveIndex;
            continue;
        }

        const tinygltf::Accessor & indexAccessor =
            model.accessors[primitive.indices];

        const tinygltf::BufferView & indexView =
            model.bufferViews[indexAccessor.bufferView];

        const tinygltf::Buffer & indexBuffer =
            model.buffers[indexView.buffer];

        const unsigned char *indexData =
            indexBuffer.data.data () +
            indexView.byteOffset +
            indexAccessor.byteOffset;

        std::vector < unsigned int > indices (
            indexAccessor.count
        );

        for (size_t i = 0;
                i < indexAccessor.count;
                ++i) {

            switch (indexAccessor.componentType) {

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:

                indices[i] =
                    reinterpret_cast <
                    const unsigned char *
                    >(indexData)[i];

                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:

                indices[i] =
                    reinterpret_cast <
                    const unsigned short *
                    >(indexData)[i];

                break;

            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:

                indices[i] =
                    reinterpret_cast <
                    const unsigned int *
                    >(indexData)[i];

                break;

            default:

                std::cerr
                        << "Unsupported index component type: "
                        << indexAccessor.componentType
                        << std::endl;

                indices.clear ();

                break;
            }

            if (indices.empty ())
                break;
        }

        if (indices.empty ()) {

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

        std::cout
                << "Creating ManualObject: "
                << objectName
                << std::endl;

        Ogre::ManualObject *manual =
            sceneManager->createManualObject();
        manual->setName(objectName);
//-----------------------------------------------------
// GLTF material
//-----------------------------------------------------

        Ogre::ColourValue baseColour =
            Ogre::ColourValue::White;

        bool hasGltfMaterial = false;

        if (primitive.material >= 0 &&
                primitive.material <
                static_cast<int>(model.materials.size())) {
            const tinygltf::Material &material =
                model.materials[primitive.material];

            const auto &factor =
                material.pbrMetallicRoughness.baseColorFactor;

            std::cout
                    << "Material "
                    << primitive.material
                    << " baseColorFactor = ";

            for (double v : factor)
                std::cout << v << " ";

            std::cout << std::endl;

            if (factor.size() >= 4) {
                baseColour =
                    Ogre::ColourValue(
                        static_cast<float>(factor[0]),
                        static_cast<float>(factor[1]),
                        static_cast<float>(factor[2]),
                        static_cast<float>(factor[3])
                    );

                hasGltfMaterial = true;

                std::cout
                        << "GLTF material "
                        << primitive.material
                        << " baseColorFactor = "
                        << factor[0]
                        << ", "
                        << factor[1]
                        << ", "
                        << factor[2]
                        << ", "
                        << factor[3]
                        << std::endl;
            }
        } else {
            std::cout
                    << "Primitive "
                    << primitiveIndex
                    << " has no valid material"
                    << std::endl;
        }

//-----------------------------------------------------
// Get PBS HLMS
//-----------------------------------------------------

        Ogre::HlmsPbs *hlmsPbs =
            mRenderer.getHlmsPbs();

        if (!hlmsPbs) {
            std::cerr
                    << "ERROR: HLMS PBS is NULL!"
                    << std::endl;

            sceneManager->destroyManualObject(manual);
            return;
        }

//-----------------------------------------------------
// Get the base PBS datablock
//-----------------------------------------------------

        Ogre::HlmsDatablock *defaultPbsDatablock =
            hlmsPbs->getDatablock("GltfPbsTest");

        if (!defaultPbsDatablock) {
            std::cerr
                    << "ERROR: GltfPbsTest datablock not found!"
                    << std::endl;

            sceneManager->destroyManualObject(manual);
            return;
        }
        std::cout
                << "PBS datablock ptr = "
                << static_cast<void*>(defaultPbsDatablock)
                << std::endl;

//-----------------------------------------------------
// Create/select material-specific PBS datablock
//-----------------------------------------------------

        Ogre::HlmsDatablock *datablock =
            defaultPbsDatablock;

        if (hasGltfMaterial) {
            const std::string materialDatablockName =
                meshName
                + "_GltfPbsMaterial_"
                + std::to_string(primitive.material);

            //-------------------------------------------------
            // Does this material datablock already exist?
            //-------------------------------------------------

            datablock =
                hlmsPbs->getDatablock(
                    materialDatablockName
                );

            //-------------------------------------------------
            // Create it if necessary
            //-------------------------------------------------

            if (!datablock) {
                datablock =
                    defaultPbsDatablock->clone(
                        materialDatablockName
                    );

                if (!datablock) {
                    std::cerr
                            << "ERROR: Could not clone GltfPbsTest "
                            << "for material "
                            << primitive.material
                            << std::endl;

                    sceneManager->destroyManualObject(manual);
                    return;
                }

                std::cout
                        << "Created PBS material datablock: "
                        << materialDatablockName
                        << std::endl;
            }

            //-------------------------------------------------
            // Convert to PBS datablock
            //-------------------------------------------------

            Ogre::HlmsPbsDatablock *pbsDatablock =
                dynamic_cast<Ogre::HlmsPbsDatablock *>(
                    datablock
                );

            if (!pbsDatablock) {
                std::cerr
                        << "ERROR: Datablock '"
                        << materialDatablockName
                        << "' is not an HlmsPbsDatablock!"
                        << std::endl;

                sceneManager->destroyManualObject(manual);
                return;
            }

            //-------------------------------------------------
            // Apply glTF baseColorFactor
            //-------------------------------------------------

            pbsDatablock->setDiffuse(
                Ogre::Vector3(
                    baseColour.r,
                    baseColour.g,
                    baseColour.b
                )
            );

            //-------------------------------------------------
            // glTF alpha
            //-------------------------------------------------

            // For now we keep the default PBS transparency
            // handling. Alpha can be implemented separately.
            //
            // pbsDatablock->setTransparency(...);

            //-------------------------------------------------
            // Roughness
            //-------------------------------------------------

            pbsDatablock->setRoughness(
                0.6f
            );

            //-------------------------------------------------
            // Metallic
            //-------------------------------------------------

            pbsDatablock->setMetalness(
                0.0f
            );

            std::cout
                    << "Applied PBS material colour: "
                    << baseColour.r
                    << ", "
                    << baseColour.g
                    << ", "
                    << baseColour.b
                    << ", "
                    << baseColour.a
                    << std::endl;
        }

//-----------------------------------------------------
// Begin ManualObject
//-----------------------------------------------------

        std::cout
                << "NameStr: "
                << *datablock->getNameStr()
                << std::endl;

        manual->begin(
            *datablock->getNameStr(),
            Ogre::OT_TRIANGLE_LIST
        );

//-----------------------------------------------------
// Vertices
//-----------------------------------------------------

        for (size_t i = 0;
                i < positions.size();
                ++i) {
            manual->position(
                positions[i]
            );

            if (i < normals.size()) {
                manual->normal(
                    normals[i]
                );
            }

            if (i < uvs.size()) {
                manual->textureCoord(
                    uvs[i]
                );
            }
        }

//-----------------------------------------------------
// Indices
//-----------------------------------------------------

        for (unsigned int index : indices) {
            manual->index(index);
        }

//-----------------------------------------------------
// Finish ManualObject
//-----------------------------------------------------

        Ogre::ManualObject::ManualObjectSection *section =
            manual->end();

        if (section && datablock) {
            section->setDatablock(datablock);

            std::cout
                    << "Using PBS datablock pointer: "
                    << datablock
                    << " name="
                    << *datablock->getNameStr()
                    << std::endl;
        }

//-----------------------------------------------------
// Finalize
//-----------------------------------------------------

        manual->setCastShadows(true);

        manual->setVisibilityFlags(
            0xFFFFFFFF
        );

//-----------------------------------------------------
// Attach object
//-----------------------------------------------------
        /* eredeti, szetesett
        Ogre::SceneNode *node =
            sceneManager->getRootSceneNode()
                ->createChildSceneNode();

        node->attachObject(manual);
        */
        /*Javasolt 1
        Ogre::SceneNode *node = parentNode;

        if (!node)
        {
            node =
                sceneManager->getRootSceneNode()
                    ->createChildSceneNode();
        }

        node->attachObject(manual);
        */
        if (!parentNode) {
            std::cerr
                    << "ERROR: buildMesh() received NULL parentNode!"
                    << std::endl;

            sceneManager->destroyManualObject(manual);
            return;
        }

        parentNode->attachObject(manual);


//-----------------------------------------------------
// Debug information
//-----------------------------------------------------

        std::cout
                << "  Created Ogre ManualObject:"
                << std::endl
                << "    name="
                << manual->getName()
                << std::endl
                << "    vertices="
                << positions.size()
                << std::endl
                << "    indices="
                << indices.size()
                << std::endl
                << "    attached="
                << manual->isAttached()
                << std::endl;

        ++primitiveIndex;
    }
}
