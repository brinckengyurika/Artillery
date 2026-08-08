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
    mResources( *this ),
    mScene( *this ),
    mGltfLoader(*this),
    mMeshBuilder( *this ) {
}

Renderer::~Renderer() {
    delete mRoot;
}


bool Renderer::initialize() {
    std::cout << "Creating Ogre Root..." << std::endl;

    mRoot = new Ogre::Root(
        ARTILLERY_CFG_DIR "/plugins.cfg",
        "",
        "Ogre.log"
    );

    const Ogre::RenderSystemList &renderers =
        mRoot->getAvailableRenderers();

    if( renderers.empty() ) {
        std::cerr << "No RenderSystems found!" << std::endl;
        return false;
    }

    Ogre::RenderSystem *rs = renderers.front();
    rs->setConfigOption( "Full Screen", "No" );
    rs->setConfigOption( "Video Mode", "1280x720" );
    rs->setConfigOption( "VSync", "No" );
    mRoot->setRenderSystem( rs );

    mWindow = mRoot->initialise(
                  true,
                  "Artillery"
              );
    //-------------------------------------------------------
    // Native X11 handles
    //-------------------------------------------------------

    size_t windowHandle = 0;
    mWindow->getCustomAttribute( "WINDOW", &windowHandle );

    size_t displayHandle = 0;
    mWindow->getCustomAttribute( "DISPLAY", &displayHandle );

    std::cout << "Window handle  = "
              << windowHandle
              << std::endl;

    std::cout << "Display handle = "
              << displayHandle
              << std::endl;

    Display *display =
        reinterpret_cast<Display *>(displayHandle);

    ::Window window =
        static_cast<::Window>(windowHandle);

    if( !mInputManager.initialize(display, window) )
        return false;
    //-------------------------------------------------------
    // Resources
    //-------------------------------------------------------

    if( !mResources.initialize() )
        return false;

    //-------------------------------------------------------
    // Scene Manager
    //-------------------------------------------------------

    mSceneManager = mRoot->createSceneManager(
                        Ogre::ST_GENERIC,
                        1
                    );
    //-------------------------------------------------------
    // Camera
    //-------------------------------------------------------
    mCamera =
        mSceneManager->createCamera( "MainCamera" );

    mCamera->setNearClipDistance( 0.1f );
    mCamera->setFarClipDistance( 100000.0f );
    mCamera->setAutoAspectRatio( true );

    mCamera->setPosition( 0.0f, 2.0f, 8.0f );

    mCameraController.initialize( mCamera );

    //-------------------------------------------------------
    // Workspace
    //-------------------------------------------------------
    Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();
    const Ogre::String workspaceDefName = "Default Workspace";
    if (!compositorManager->hasWorkspaceDefinition(workspaceDefName))    {
        // 3. Ha az Ogre alapértelmezett beállításait szeretnéd használni (Clear + Render Scene)
        // Ez automatikusan létrehozza a belső Node-okat és magát a definíciót is.
        compositorManager->createBasicWorkspaceDef(workspaceDefName, Ogre::ColourValue::Blue);
    }
    mWorkspace = compositorManager->addWorkspace(
                     mSceneManager,
                     mWindow->getTexture(),
                     mCamera,
                     workspaceDefName,
                     true
                 );

    //-------------------------------------------------------
    // Scene
    //-------------------------------------------------------

    if( !mScene.initialize() )
        return false;
    std::cout << "Renderer initialized successfully." << std::endl;
    const std::string modelname = "Eiffel3";
    const std::string modelpath = "/home/satch/Projects/Artillery/media/models/" + modelname + ".glb";

    tinygltf::Model model;
    //Ez az eredeti:
    //mGltfLoader.load(ARTILLERY_MEDIA_DIR "/models/Apollo.glb", model);

    mGltfLoader.load(modelpath, model);
    mMeshBuilder.inspect(model);

    mMeshBuilder.build(
        model,
        mSceneManager,
        mSceneManager->getRootSceneNode(),
        modelname
    );



    Ogre::Item *item =
        mSceneManager->createItem(modelname);

    Ogre::SceneNode *node =
        mSceneManager->getRootSceneNode()->createChildSceneNode();

    node->attachObject(item);

    // ideiglenesen tegyük a világ origójába
    node->setPosition(0.0f, 0.0f, 0.0f);



    std::cout << "Camera parent node   = "
              << mCamera->getParentNode()
              << std::endl;


    return true;
}

bool Renderer::renderFrame() {

    Ogre::WindowEventUtilities::messagePump();

    mInputManager.update();


    if( mInputManager.shouldQuit() )
        return false;

    if( mWindow->isClosed() )
        return false;


    mCameraController.update(
        mInputManager,
        0.016f );

    return mRoot->renderOneFrame();
}


void Renderer::shutdown() {
    mInputManager.shutdown();
    mScene.shutdown();
    mResources.shutdown();

    std::cout << "Renderer shutdown()" << std::endl;
}
