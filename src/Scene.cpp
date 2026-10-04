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
    std::cout << "Create light" << std::endl;
    int sceneindex = 0;
    if( !createLight() )
        return false;
//    if( !createObjects() )
//        return false;
    std::cout << "Create object" << std::endl;
    if (!createObjectsNew(sceneindex))
            return false;
    std::cout << "Create terra" << std::endl;
    if (!createTerra(sceneindex))
        return false;
    std::cout << "Scene initialisation done" << std::endl;
    return true;
}

bool Scene::createLight() {
    Ogre::SceneManager *scene =
        mRenderer.getSceneManager();
    int lighcount = mRenderer.getConfig().getLights().size();
    for (int lc = 0; lc < lighcount; ++lc) {
        Ogre::SceneNode *nodeOriginalLight =
            scene->getRootSceneNode()->createChildSceneNode();
        Ogre::Light *light =
            scene->createLight();
        light->setName(mRenderer.getConfig().getLights()[lc].getName());
        if ( mRenderer.getConfig().getLights()[lc].getType() == "LT_DIRECTIONAL") {
            light->setType( Ogre::Light::LT_DIRECTIONAL );
        } else {
            std::cout << "THE TYPE OF THE LIGHT IS NOT RECOGNIZABLE!" << std::endl;
        }

        light->setDiffuseColour(
            Ogre::ColourValue(
                mRenderer.getConfig().getLights()[lc].getColor()[0],
                mRenderer.getConfig().getLights()[lc].getColor()[1],
                mRenderer.getConfig().getLights()[lc].getColor()[2]
            )
        );
        light->setSpecularColour(
            Ogre::ColourValue(
                mRenderer.getConfig().getLights()[lc].getSpecularColor()[0],
                mRenderer.getConfig().getLights()[lc].getSpecularColor()[1],
                mRenderer.getConfig().getLights()[lc].getSpecularColor()[2]
            )
        );
        nodeOriginalLight->attachObject( light );
        nodeOriginalLight->setDirection(
            Ogre::Vector3(
//             -1, -1, -1
                mRenderer.getConfig().getLights()[lc].getDirection()[0],
                mRenderer.getConfig().getLights()[lc].getDirection()[1],
                mRenderer.getConfig().getLights()[lc].getDirection()[2]
             ).normalisedCopy()
        );
    }

    std::cout << "-----------------------Scene Createlighy finished" << std::endl;
    return true;
}


bool Scene::createObjectsNew(int sceneindex) {
    //-------------------------------------------------------
    // GLTF model
    //-------------------------------------------------------
    //int sceneindex = 0;
    int ol = mRenderer.getConfig().getScenes()[sceneindex].getModels().size();
    std::string err;
    std::string warn;
    std::string name;

    bool loaded;
    tinygltf::TinyGLTF loader;
    std::cout << "Betoltendo objectek szama: " << ol << std::endl;
    for (int i = 0; i < ol; ++i) {
        name = mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getModelName();
        std::cout << "Betoltendo object: " << name << std::endl;
        tinygltf::Model model;
        const auto& data = mRenderer.getConfig().getBinaryFile(name);
        loaded =
            loader.LoadBinaryFromMemory(
                &model,
                &err,
                &warn,
                data.data(),
                data.size()
            );
        if(!loaded) {
            std::cerr
                    << "Failed to load GLB: "
                    << name
                    << std::endl;

            return false;
        }

        const std::string filename =
            ARTILLERY_MEDIA_DIR "/models/2CylinderEngine.glb";
        //-------------------------------------------------------
        // Create parent node
        //-------------------------------------------------------


        Ogre::SceneNode *parentNode =
            mRenderer.getSceneManager()
            ->getRootSceneNode()
            ->createChildSceneNode();
        parentNode->setPosition(
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getPosition()[0],
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getPosition()[1],
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getPosition()[2]
        );

        parentNode->setScale(
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getScale()[0],
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getScale()[1],
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getScale()[2]
        );

        parentNode->roll(Ogre::Degree(
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getRotation()[2]
            )
        );
        parentNode->yaw(Ogre::Degree(
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getRotation()[1]
            )
        );
        parentNode->pitch(Ogre::Degree(
            mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getRotation()[0]
            )
        );




        //-------------------------------------------------------
        // Build GLTF
        //-------------------------------------------------------


        GltfMeshBuilder builder(mRenderer);
        if(!builder.build(
                    model,
                    mRenderer.getSceneManager(),
                    parentNode,
                    mRenderer.getConfig().getScenes()[sceneindex].getModels()[i].getName())) {
            std::cerr << "Failed to build GLTF model." << std::endl;
            return false;
        }
        std::cout << "Betoltott object: " << name << std::endl;
    }
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
        "heightmap3.png",
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



bool Scene::createTerra(int sceneindex) {
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
    const std::string& terrain_ref = mRenderer.getConfig().getScenes()[sceneindex].getTerrain();
    const TerrainConfig* terrainconfig = mRenderer.getConfig().getTerrainConfigByName(terrain_ref);
    if (!terrainconfig)    {
    // nincs ilyen terrain
    // hibakezelés
        return false;
    }

    const TerrainConfig& terrain = *terrainconfig;
    std::cout<< " ==================================== EZEGYDEBUG: " << terrain.getHeightmap() <<std::endl;
    mTerra->load(
        terrain.getHeightmap(),
        Ogre::Vector3(
            terrain.getPosition()[0],
            terrain.getPosition()[1],
            terrain.getPosition()[2]
        ),
        Ogre::Vector3(
            terrain.getSize()[0],
            terrain.getSize()[1],
            terrain.getSize()[2]
        ),
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
