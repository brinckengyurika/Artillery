#include "ResourceManager.h"

#include "Renderer.h"

#include <iostream>
#include <OgreArchiveManager.h>
#include <OgreConfigFile.h>
#include <OgreHlms.h>
#include <OgreHlmsManager.h>
#include <OgreResourceGroupManager.h>

#include <Hlms/Unlit/OgreHlmsUnlit.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>

#include <OgreHlmsDatablock.h>
#include <OgrePrerequisites.h>
#include <Hlms/Unlit/OgreHlmsUnlitDatablock.h>
#include <Hlms/Pbs/OgreHlmsPbsDatablock.h>

#include "Terra/Hlms/OgreHlmsTerra.h"
#include <OgreMaterialManager.h>

ResourceManager::ResourceManager(Renderer &renderer) :
    mRenderer(renderer) {
}

bool ResourceManager::initialize() {
    if( !setupResources() )
        return false;
    if( !registerHlms() )
        return false;

    Ogre::HlmsDatablock *test =
        mRenderer.getRoot()
        ->getHlmsManager()
        ->getDatablock("GltfDefault");

    if( test ) {
        std::cout
                << "HLMS lookup OK: "
                << test->getNameStr()
                << " ptr="
                << test
                << std::endl;
    } else {
        std::cout
                << "ERROR: GltfDefault not found in HlmsManager!"
                << std::endl;
    }




    Ogre::MaterialManager &mm =
        Ogre::MaterialManager::getSingleton();

    std::cout
            << "Material Terra/GpuNormalMapper exists: "
            << mm.resourceExists("Terra/GpuNormalMapper")
            << std::endl;

    std::cout
            << "Material GpuNormalMapper exists: "
            << mm.resourceExists("GpuNormalMapper")
            << std::endl;




    Ogre::ResourceGroupManager &rgm =
        Ogre::ResourceGroupManager::getSingleton();

    std::cout << "=== RESOURCE GROUPS ===" << std::endl;

    Ogre::StringVector groups =
        rgm.getResourceGroups();

    for( const auto &group : groups ) {
        std::cout << "GROUP: " << group << std::endl;
    }


    std::cout << "=== Terra resources ===" << std::endl;

    Ogre::StringVectorPtr resources =
        rgm.findResourceNames(
            Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
            "GpuNormalMapper*"
        );

    if( resources ) {
        for( const auto &name : *resources )
            std::cout << "RESOURCE: " << name << std::endl;
    }

    return true;
}

bool ResourceManager::registerHlms() {
    std::cout << "========================================\n";
    std::cout << "ResourceManager::registerHlms()\n";
    std::cout << "========================================\n";
    Ogre::ConfigFile cf;
    cf.load( ARTILLERY_CFG_DIR "/resources2.cfg" );

    Ogre::String rootHlmsFolder =
        cf.getSetting( "DoNotUseAsResource", "Hlms" );

    if( rootHlmsFolder.empty() )
        return false;

    if( rootHlmsFolder.back() != '/' )
        rootHlmsFolder += "/";

    Ogre::ArchiveManager &archiveManager =
        Ogre::ArchiveManager::getSingleton();

    const Ogre::String archiveType = "FileSystem";

    //
    // HLMS Unlit
    //
    Ogre::String mainFolder;
    Ogre::StringVector libraryFolders;

    Ogre::HlmsUnlit::getDefaultPaths(
        mainFolder,
        libraryFolders
    );

    Ogre::Archive *archiveUnlit =
        archiveManager.load(
            rootHlmsFolder + mainFolder,
            archiveType,
            true );

    Ogre::ArchiveVec archiveUnlitLibraries;

    for( const auto &folder : libraryFolders ) {
        archiveUnlitLibraries.push_back(
            archiveManager.load(
                rootHlmsFolder + folder,
                archiveType,
                true ) );
    }

    Ogre::HlmsUnlit *hlmsUnlit =
        OGRE_NEW Ogre::HlmsUnlit(
            archiveUnlit,
            &archiveUnlitLibraries );

    std::cout << "Registering HLMS Unlit..." << std::endl;
    mRenderer.getRoot()
    ->getHlmsManager()
    ->registerHlms( hlmsUnlit );
    std::cout << "HLMS Unlit registered." << std::endl;

    Ogre::HlmsMacroblock macroblock;
    Ogre::HlmsBlendblock blendblock;
    Ogre::HlmsParamVec params;

    Ogre::HlmsDatablock *defaultDatablock =
        hlmsUnlit->createDatablock(
            "GltfDefault",
            "GltfDefault",
            macroblock,
            blendblock,
            params
        );

    if( !defaultDatablock ) {
        std::cerr
                << "ERROR: Failed to create GltfDefault datablock!"
                << std::endl;

        return false;
    }

    Ogre::HlmsUnlitDatablock *unlit =
        static_cast<Ogre::HlmsUnlitDatablock *>(
            defaultDatablock
        );

    unlit->setUseColour(true);
    unlit->setColour(Ogre::ColourValue::White);

    std::cout << "UNLIT datablock: " ;
    std::cout << "hasColour=" << unlit->hasColour();
    std::cout << " colour="
              << unlit->getColour().r << ", "
              << unlit->getColour().g << ", "
              << unlit->getColour().b << ", "
              << unlit->getColour().a
              << std::endl;
    /*
    unlitDatablock->setColour(
        Ogre::ColourValue( 1.0f, 0.0f, 0.0f, 1.0f )
    );
    */
    Ogre::HlmsDatablock *test =
        hlmsUnlit->getDatablock("GltfDefault");

    if(test) {
        std::cout
                << "GltfDefault FOUND in Unlit HLMS"
                << std::endl;
    } else {
        std::cout
                << "GltfDefault NOT FOUND in Unlit HLMS"
                << std::endl;
    }
    std::cout
            << "Created HLMS datablock: GltfDefault"
            << std::endl;

    //
    // HLMS PBS
    //
    mRenderer.setHlmsUnlit(hlmsUnlit);

    libraryFolders.clear();

    Ogre::HlmsPbs::getDefaultPaths(
        mainFolder,
        libraryFolders );


    Ogre::Archive *archivePbs =
        archiveManager.load(
            rootHlmsFolder + mainFolder,
            archiveType,
            true );

    Ogre::ArchiveVec archivePbsLibraries;

    for( const auto &folder : libraryFolders ) {
        archivePbsLibraries.push_back(
            archiveManager.load(
                rootHlmsFolder + folder,
                archiveType,
                true ) );
    }

    Ogre::HlmsPbs *hlmsPbs =
        OGRE_NEW Ogre::HlmsPbs(
            archivePbs,
            &archivePbsLibraries );

    std::cout << "Registering HLMS PBS..." << std::endl;
    mRenderer.getRoot()
    ->getHlmsManager()
    ->registerHlms( hlmsPbs );
    std::cout << "HLMS PBS registered." << std::endl;
    mRenderer.setHlmsPbs(hlmsPbs);
//---------------------------------------------------------
// Create default PBS datablock for glTF meshes
//---------------------------------------------------------


//
// HLMS TERRA
//

    Ogre::String terraMainFolder;
    Ogre::StringVector terraLibraryFolders;

    Ogre::HlmsTerra::getDefaultPaths(
        terraMainFolder,
        terraLibraryFolders
    );

    std::cout << "Terra main folder: "
              << terraMainFolder
              << std::endl;



    Ogre::ResourceGroupManager &rgm =
        Ogre::ResourceGroupManager::getSingleton();

    std::cout
            << "GpuNormalMapper exists: "
            << rgm.resourceExists(
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                "GpuNormalMapper.material" )
            << std::endl;



    std::cout
            << "Terra/GpuNormalMapper exists: "
            << rgm.resourceExists(
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                "Terra/GpuNormalMapper" )
            << std::endl;




    Ogre::Archive *archiveTerra =
        archiveManager.load(
            rootHlmsFolder + terraMainFolder,
            archiveType,
            true
        );

    Ogre::ArchiveVec archiveTerraLibraries;

    for( const auto &folder : terraLibraryFolders ) {
        archiveTerraLibraries.push_back(
            archiveManager.load(
                rootHlmsFolder + folder,
                archiveType,
                true
            )
        );
    }

    Ogre::HlmsTerra *hlmsTerra =
        OGRE_NEW Ogre::HlmsTerra(
            archiveTerra,
            &archiveTerraLibraries
        );


std::cout << "Registering HLMS Terra..." << std::endl;

mRenderer.getRoot()
    ->getHlmsManager()
    ->registerHlms( hlmsTerra );

std::cout << "HLMS Terra registered." << std::endl;

mRenderer.setHlmsTerra( hlmsTerra );

mHlmsTerra = hlmsTerra;

Ogre::HlmsDatablock *terraDatablock =
    hlmsTerra->createDatablock(
        Ogre::IdString("TerraExampleMaterial"),
        "TerraExampleMaterial",
        macroblock,
        blendblock,
        params
    );

if( !terraDatablock )
{
    std::cerr << "ERROR: Cannot create Terra datablock!"
              << std::endl;
    return false;
}

mTerraDatablock = terraDatablock;


std::cout
    << "Created Terra datablock: TerraDatablock"
    << std::endl;


    Ogre::HlmsParamVec paramVec;

    Ogre::HlmsDatablock *datablock =
        hlmsPbs->createDatablock(
            Ogre::IdString("GltfPbsTest"),
            "GltfPbsTest",
            macroblock,
            blendblock,
            paramVec
        );

    if (!datablock) {
        std::cerr
                << "ERROR: Failed to create GltfPbsTest datablock!"
                << std::endl;

        return false;
    }


// Átalakítás PBS datablock-ra
    Ogre::HlmsPbsDatablock *pbsDatablock =
        dynamic_cast<Ogre::HlmsPbsDatablock *>(datablock);

    if (!pbsDatablock) {
        std::cerr
                << "ERROR: GltfPbsTest is not an HlmsPbsDatablock!"
                << std::endl;

        return false;
    }


    pbsDatablock->setDiffuse(
        Ogre::Vector3(1.0f, 1.0f, 1.0f)
    );

    pbsDatablock->setSpecular(
        Ogre::Vector3(0.5f, 0.5f, 0.5f)
    );

    pbsDatablock->setRoughness(0.6f);

    std::cout
            << "Created PBS datablock: GltfPbsTest"
            << std::endl;

    return true;

}

bool ResourceManager::setupResources() {
    Ogre::ConfigFile cf;


    cf.load( ARTILLERY_CFG_DIR "/resources2.cfg" );

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName;
    Ogre::String typeName;
    Ogre::String archName;

    while( seci.hasMoreElements() ) {
        secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *settings =
            seci.getNext();

        if( secName == "Hlms" )
            continue;

        for( auto &entry : *settings ) {
            typeName = entry.first;
            archName = entry.second;

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName,
                typeName,
                secName
            );
        }
    }
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        ARTILLERY_MEDIA_DIR,
        "FileSystem",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
    );

    //---------------------------------------------------------
    // Initialize resource groups
    //---------------------------------------------------------

    Ogre::ResourceGroupManager &rgm =
        Ogre::ResourceGroupManager::getSingleton();


    std::cout << "initialiseResourceGroup Start" << std::endl;
    rgm.initialiseResourceGroup("Essential", False);
    rgm.initialiseResourceGroup("Popular", False);
    rgm.initialiseResourceGroup("General", False);

    std::cout << "initialiseResourceGroup End" << std::endl;
    return true;
}

void ResourceManager::shutdown() {
    std::cout << "Shutting down resources..." << std::endl;
}
