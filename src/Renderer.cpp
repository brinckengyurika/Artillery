#include "Renderer.h"
#include <OgreLight.h>
#include <iostream>
#include <OgreItem.h>
#include <OgreColourValue.h>
#include <OgreLight.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager2.h>
#include <OgreResourceManager.h>
#include <OgreArchiveManager.h>
#include "tiny_gltf.h"
#include <vector>
#include <map>

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
    mWindow->getCustomAttribute(
        "WINDOW",
        &windowHandle
    );

    size_t displayHandle = 0;
    mWindow->getCustomAttribute(
        "DISPLAY",
        &displayHandle
    );

    std::cout
            << "Window handle  = "
            << windowHandle
            << std::endl;

    std::cout
            << "Display handle = "
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

    mSceneManager =
        mRoot->createSceneManager(
            Ogre::ST_GENERIC,
            1
        );

    //-------------------------------------------------------
    // Camera
    //-------------------------------------------------------

    mCamera =
        mSceneManager->createCamera(
            "MainCamera"
        );

    mCamera->setNearClipDistance( 0.1f );
    mCamera->setFarClipDistance( 100000.0f );
    mCamera->setAutoAspectRatio( true );

    mCamera->setPosition(
        0.0f,
        2.0f,
        8.0f
    );

    mCameraController.initialize(
        mCamera
    );

    //-------------------------------------------------------
    // Workspace
    //-------------------------------------------------------

    Ogre::CompositorManager2 *compositorManager =
        mRoot->getCompositorManager2();

    const Ogre::String workspaceDefName =
        "Default Workspace";

    if( !compositorManager->hasWorkspaceDefinition(
                workspaceDefName ) ) {
        compositorManager->createBasicWorkspaceDef(
            workspaceDefName,
            Ogre::ColourValue::White
        );
    }

    mWorkspace =
        compositorManager->addWorkspace(
            mSceneManager,
            mWindow->getTexture(),
            mCamera,
            workspaceDefName,
            true
        );

/*Refactoring

    mTerra =
        new Ogre::Terra(
        Ogre::Id::generateNewId<Ogre::MovableObject>(),
        &mSceneManager->_getEntityMemoryManager(
            Ogre::SCENE_STATIC ),
        mSceneManager,
        11u,
        mRoot->getCompositorManager2(),
        mCamera,
        false
    );
*/
    Ogre::SceneNode *terrainNode =
        mSceneManager->getRootSceneNode(
            Ogre::SCENE_STATIC
        )->createChildSceneNode(
            Ogre::SCENE_STATIC
        );
/*refactoring
    terrainNode->attachObject( mTerra );
*/
    //-------------------------------------------------------
    // Scene
    //-------------------------------------------------------
/*refactoring
    std::cout << "load heightmap start" << std::endl;
    mTerra->load(
        "Heightmap.png",
        Ogre::Vector3(0.0f, 500.0f, 0.0f),
        Ogre::Vector3(4096.0f, 1000.0f, 4096.0f),
        false,
        false
    );
*/

    /*
        Ogre::HlmsDatablock *terraDatablock =
            mResources.getTerraDatablock();

        std::cout << "Terra datablock: "
                  << terraDatablock << std::endl;

        if( !terraDatablock ) {
            std::cerr << "ERROR: Terra datablock is NULL!" << std::endl;
            return false;
        }


        mTerra->setDatablock( terraDatablock );
    */

    Ogre::HlmsDatablock *terraDatablock =
        mResources.getTerraDatablock();

    if( !terraDatablock ) {
        std::cerr << "ERROR: Terra datablock is NULL!"
                  << std::endl;
        return false;
    }
/*Refactoring
    mTerra->setDatablock( terraDatablock );
*/
    std::cout
            << "Terra datablock assigned."
            << std::endl;


    Ogre::HlmsDatablock *db =
        mResources.getTerraDatablock();

    std::cout << "\n========== TERRA DEBUG ==========\n";


    std::cout << "Datablock ptr   = " << db << "\n";

    if( db ) {
        std::cout << "Datablock name  = "
                  << db->getNameStr() << "\n";

        std::cout << "Datablock creator = "
                  << db->getCreator() << "\n";
    }
/*refactoring
    std::cout << "Visible         = "
              << mTerra->getVisible() << "\n";

    std::cout << "XZ dimensions   = "
              << mTerra->getXZDimensions() << "\n";

    std::cout << "World radius    = "
              << mTerra->getWorldRadius() << "\n";

    const Ogre::Aabb &aabb =
        mTerra->getWorldAabbUpdated();

    std::cout << "World AABB min  = "
              << aabb.getMinimum() << "\n";

    std::cout << "World AABB max  = "
              << aabb.getMaximum() << "\n";

    std::cout << "=================================\n";

*/
    /*arnyek kikapcsolasa ideiglenesen

    */
/*refavtoring
    mTerra->setCastShadows(false);





    mTerra->update(
        Ogre::Vector3( -1.0f, -1.0f, -1.0f ).normalisedCopy()
    );

    std::cout << "---- END TERRA TEST ----" << std::endl;

    std::cout << "Terra loaded." << std::endl;


    std::cout << "load heightmap end" << std::endl;
*/
    /*

        mCamera->setPosition(
            2048.0f,
            1500.0f,
            3000.0f
        );

        mCamera->lookAt(
            Ogre::Vector3(
                2048.0f,
                0.0f,
                2048.0f
            )
        );
    */

    mCamera->setPosition(0.0f, 800.0f, 1200.0f);
    mCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));



    mCamera->setNearClipDistance(1.0f);
    mCamera->setFarClipDistance(10000.0f);







    if( !mScene.initialize() )
        return false;

    return true;
    // ...
}

/*Refacotring
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



    mTerra->update(
        Ogre::Vector3(0.0f, -1.0f, 0.0f)
    );
    return mRoot->renderOneFrame();
}
*/
bool Renderer::renderFrame()
{
    float timestep = 0.016f;
    Ogre::WindowEventUtilities::messagePump();

    mInputManager.update();

    if(mInputManager.shouldQuit())
        return false;

    if(mWindow->isClosed())
        return false;

    mCameraController.update(
        mInputManager,
        timestep
    );
/*Refactoring
    mScene.update(0.016f);
*/
    mScene.update(timestep);
    return mRoot->renderOneFrame();
}




void Renderer::shutdown() {
    mInputManager.shutdown();
    mScene.shutdown();
    mResources.shutdown();

    std::cout << "Renderer shutdown()" << std::endl;
}
