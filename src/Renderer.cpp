#include "Renderer.h"

#include <iostream>
#include <OgreItem.h>
#include <OgreColourValue.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>
#include "tiny_gltf.h"

Renderer::Renderer() :
    mRoot( nullptr ),
    mWindow( nullptr ),
    mSceneManager( nullptr ),
    mCamera( nullptr ),
    mCameraNode( nullptr ),
    mResources( *this ),
    mScene( *this ),
    mGltfLoader(*this),
    mMeshBuilder( *this )
{
}

Renderer::~Renderer()
{
    delete mRoot;
}

bool Renderer::initialize()
{
    std::cout << "Creating Ogre Root..." << std::endl;

    mRoot = new Ogre::Root(
        ARTILLERY_CFG_DIR "/plugins.cfg",
        "",
        "Ogre.log"
    );

    const Ogre::RenderSystemList &renderers =
        mRoot->getAvailableRenderers();

    if( renderers.empty() )
    {
        std::cerr << "No RenderSystems found!" << std::endl;
        return false;
    }
    Ogre::RenderSystem *rs = renderers.front();

    rs->setConfigOption( "Full Screen", "No" );
    rs->setConfigOption( "Video Mode", "1280x720" );
    rs->setConfigOption( "VSync", "No" );

    mRoot->setRenderSystem( rs );

//    mRoot->setRenderSystem( renderers.front() );

    mWindow = mRoot->initialise(
        true,
        "Artillery"
    );

    std::cout
        << "Window size: "
        << mWindow->getWidth()
        << " x "
        << mWindow->getHeight()
        << std::endl;

    //-------------------------------------------------------
    // SceneManager
    //-------------------------------------------------------

    mSceneManager =
        mRoot->createSceneManager(
            Ogre::ST_GENERIC,
            1,
            "MainSceneManager"
        );

    //-------------------------------------------------------
    // Camera
    //-------------------------------------------------------

    mCamera =
        mSceneManager->createCamera(
            "MainCamera"
        );

    mCamera->setNearClipDistance( 0.2f );
    mCamera->setFarClipDistance( 1000.0f );
    mCamera->setAutoAspectRatio( true );

    mCameraNode =
        mSceneManager
            ->getRootSceneNode()
            ->createChildSceneNode();

    mCameraNode->setPosition(
        0.0f,
        5.0f,
        15.0f
    );

    mCameraNode->lookAt(
        Ogre::Vector3::ZERO,
        Ogre::Node::TS_WORLD
    );

    // Ogre 2.3 createCamera() automatikusan attach-olja
    // a kamerát a SceneNode-hoz.

    //-------------------------------------------------------
    // Resources
    //-------------------------------------------------------

    if( !mResources.initialize() )
        return false;

    //-------------------------------------------------------
    // Workspace
    //-------------------------------------------------------

    Ogre::CompositorManager2 *compositorManager =
        mRoot->getCompositorManager2();

    const Ogre::String workspaceName(
        "Artillery Workspace"
    );

    if( !compositorManager->hasWorkspaceDefinition( workspaceName ) )
    {
        compositorManager->createBasicWorkspaceDef(
            workspaceName,
            Ogre::ColourValue( 0.15f, 0.25f, 0.45f ),
            Ogre::IdString()
        );
    }

    mWorkspace =
        compositorManager->addWorkspace(
            mSceneManager,
            mWindow->getTexture(),
            mCamera,
            workspaceName,
            true
        );

    //-------------------------------------------------------
    // Camera Controller
    //-------------------------------------------------------

    mCameraController.initialize(
        mCamera,
        mCameraNode
    );

    //-------------------------------------------------------
    // Scene
    //-------------------------------------------------------

    if( !mScene.initialize() )
        return false;

    const std::string modelname = "cubes";

    const std::string modelpath = "/home/satch/Projects/Artillery/media/models/" + modelname + ".glb";

    tinygltf::Model model;
    //Ez az eredeti:
    //mGltfLoader.load(ARTILLERY_MEDIA_DIR "/models/Apollo.glb", model);
    mGltfLoader.load(modelpath, model);
    mMeshBuilder.inspect(model);


    mMeshBuilder.build( model, modelname);
    Ogre::Item *item =
    mSceneManager->createItem(modelname);

    Ogre::SceneNode *node =
    mSceneManager->getRootSceneNode()->createChildSceneNode();

    node->attachObject(item);
/*
    mGltfLoader.load(
        ARTILLERY_MEDIA_DIR "/models/untitled.glb"
    );
*/


    return true;
}

bool Renderer::renderFrame()
{
    Ogre::WindowEventUtilities::messagePump();

    if( mWindow->isClosed() )
        return false;

    mCameraController.update( 0.016f );

    return mRoot->renderOneFrame();
}

void Renderer::shutdown()
{
    mScene.shutdown();
    mResources.shutdown();

    std::cout << "Renderer shutdown()" << std::endl;
}
