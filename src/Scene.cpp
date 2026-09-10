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
    if (!createTerra())
        return false;
    return true;
}

bool Scene::createLight() {
    Ogre::SceneManager *scene =
        mRenderer.getSceneManager();
    if (True) {
        Ogre::SceneNode *nodeOriginalLight =
            scene->getRootSceneNode()->createChildSceneNode();
        Ogre::Light *light =
            scene->createLight();
        light->setName("OriginalLight");
        light->setType( Ogre::Light::LT_DIRECTIONAL );

        light->setDiffuseColour(
            Ogre::ColourValue(1.0f, 1.0f, 1.0f)
        );
        light->setSpecularColour(
            Ogre::ColourValue(1.0f, 1.0f, 1.0f)
        );
        nodeOriginalLight->attachObject( light );
        nodeOriginalLight->setDirection(
            Ogre::Vector3( -1, -1, -1 ).normalisedCopy()
        );
    }

//new light
    if (True) {
        Ogre::SceneNode *nodeSunShine =
            scene->getRootSceneNode()->createChildSceneNode();
        Ogre::Light *sunLight =
            scene->createLight();
        sunLight->setName("Sunligh");
        sunLight->setType(Ogre::Light::LT_DIRECTIONAL);

        sunLight->setDiffuseColour(
            Ogre::ColourValue(1.0f, 1.0f, 1.0f)
        );
        sunLight->setSpecularColour(
            Ogre::ColourValue(1.0f, 1.0f, 1.0f)
        );
        nodeSunShine->attachObject( sunLight );
        sunLight->setDirection(
            Ogre::Vector3(-0.5f, -1.0f, -0.3f).normalisedCopy()
        );
    }

    std::cout << "-----------------------Scene Createlighy finished" << std::endl;
    return true;
}




bool Scene::createObjects() {
    //-------------------------------------------------------
    // GLTF model
    //-------------------------------------------------------

    const std::string filename =
        ARTILLERY_MEDIA_DIR "/models/2CylinderEngine.glb";

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;

    std::string err;
    std::string warn;
    bool loaded =
        loader.LoadBinaryFromFile(
            &model,
            &err,
            &warn,
            filename
        );
    if(!loaded) {
        std::cerr
                << "Failed to load GLB: "
                << filename
                << std::endl;

        return false;
    }

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


    GltfMeshBuilder builder(mRenderer);
    if(!builder.build(
                model,
                mRenderer.getSceneManager(),
                parentNode,
                "2CylinderEngine")) {
        std::cerr << "Failed to build GLTF model." << std::endl;
        return false;
    }
    return true;
}

void Scene::update(float dt) {
    if (mTerra)
        mTerra->update(
            Ogre::Vector3(0.0f, -1.0f, 0.0f)
        );
}

void Scene::shutdown() {
}

bool Scene::createTerra() {
    Ogre::SceneManager *scene = mRenderer.getSceneManager();
    const ResourceManager& mResources = mRenderer.getResourceManager();

    mTerra =
        new Ogre::Terra(
        Ogre::Id::generateNewId<Ogre::MovableObject>(),
        &scene->_getEntityMemoryManager(            Ogre::SCENE_STATIC ),
        scene,
        11u,
        mRenderer.getRoot()->getCompositorManager2(),
        mRenderer.getCamera(),
        false
    );

    Ogre::SceneNode *terrainNode =
        scene->getRootSceneNode(
            Ogre::SCENE_STATIC
        )->createChildSceneNode(
            Ogre::SCENE_STATIC
        );
    terrainNode->attachObject( mTerra );
    std::cout << "load heightmap start" << std::endl;
    mTerra->load(
        "Heightmap.png",
        Ogre::Vector3(0.0f, 500.0f, 0.0f),
        Ogre::Vector3(4096.0f, 1000.0f, 4096.0f),
        false,
        false
    );

    Ogre::HlmsDatablock *terraDatablock =
        mResources.getTerraDatablock();
    if( !terraDatablock ) {
        std::cerr << "ERROR: Terra datablock is NULL!"
                  << std::endl;
        return false;
    }
    std::cout << "Terra datablock: "
              << terraDatablock << std::endl;

    if( !terraDatablock ) {
        std::cerr << "ERROR: Terra datablock is NULL!" << std::endl;
        return false;
    }
    mTerra->setDatablock( terraDatablock );

    return true;
}
