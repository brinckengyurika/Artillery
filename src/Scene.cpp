#include "Scene.h"

#include "Renderer.h"
#include "MeshFactory.h"


#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreItem.h>
#include <iostream>

#include "GltfMeshBuilder.h"
#include <tiny_gltf.h>
#include <string>

Scene::Scene( Renderer &renderer ) :
    mRenderer( renderer ),
    mMeshFactory( renderer ) {
}

bool Scene::initialize() {
    if( !createLight() )
        return false;

    if( !createObjects() )
        return false;

    return true;
}

bool Scene::createLight() {
    Ogre::SceneManager *scene = mRenderer.getSceneManager();

    Ogre::Light *light =
        scene->createLight();

    light->setType( Ogre::Light::LT_DIRECTIONAL );

    Ogre::SceneNode *node =
        scene->getRootSceneNode()->createChildSceneNode();

    node->setDirection(
        Ogre::Vector3( -1, -1, -1 ).normalisedCopy()
    );

    node->attachObject( light );

    return true;
}
bool Scene::createObjects()
{
    //-------------------------------------------------------
    // GLTF model
    //-------------------------------------------------------

    const std::string filename =
        ARTILLERY_MEDIA_DIR "/models/2CylinderEngine.glb";

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;

    std::cout
        << "Start load."
        << std::endl;

    bool loaded =
        loader.LoadBinaryFromFile(
            &model,
            &err,
            &warn,
            filename
        );

    if(!warn.empty())
    {
        std::cout
            << "GLTF warning: "
            << warn
            << std::endl;
    }

    if(!err.empty())
    {
        std::cerr
            << "GLTF error: "
            << err
            << std::endl;
    }

    if(!loaded)
    {
        std::cerr
            << "Failed to load GLB: "
            << filename
            << std::endl;

        return false;
    }

    std::cout
        << "Loaded GLB: "
        << filename
        << std::endl;

    std::cout
        << "Stop load."
        << std::endl;


    //-------------------------------------------------------
    // Create parent node
    //-------------------------------------------------------

    Ogre::SceneNode *parentNode =
        mRenderer.getSceneManager()
        ->getRootSceneNode()
        ->createChildSceneNode();

    parentNode->setPosition(
        0.0f,
        0.0f,
        0.0f
    );


    //-------------------------------------------------------
    // Build GLTF
    //-------------------------------------------------------

    std::cout
        << "Start build."
        << std::endl;

    GltfMeshBuilder builder(mRenderer);

    if(!builder.build(
            model,
            mRenderer.getSceneManager(),
            parentNode,
            "2CylinderEngine"))
    {
        std::cerr
            << "Failed to build GLTF model."
            << std::endl;

        return false;
    }

    std::cout
        << "Stop build."
        << std::endl;


    return true;
}


void Scene::shutdown() {
}
