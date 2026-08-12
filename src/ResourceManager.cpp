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


ResourceManager::ResourceManager(Renderer &renderer) :
    mRenderer(renderer)
{
}

 bool ResourceManager::initialize()
{
    if( !setupResources() )
        return false;
    if( !registerHlms() )
        return false;

    return true;
}

bool ResourceManager::registerHlms()
{
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

    for( const auto &folder : libraryFolders )
    {
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
    //
    // HLMS PBS
    //

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

    for( const auto &folder : libraryFolders )
    {
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
    return true;
}

bool ResourceManager::setupResources()
{
    Ogre::ConfigFile cf;


    cf.load( ARTILLERY_CFG_DIR "/resources2.cfg" );

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName;
    Ogre::String typeName;
    Ogre::String archName;

    while( seci.hasMoreElements() )
    {
        secName = seci.peekNextKey();

        Ogre::ConfigFile::SettingsMultiMap *settings =
            seci.getNext();

        if( secName == "Hlms" )
            continue;

        for( auto &entry : *settings )
        {
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
    return true;
}

void ResourceManager::shutdown()
{
    std::cout << "Shutting down resources..." << std::endl;
}
