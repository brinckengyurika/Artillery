#include "MeshFactory.h"

#include "Renderer.h"

#include <OgreSceneManager.h>
#include <OgreItem.h>
#include <OgreResourceGroupManager.h>

MeshFactory::MeshFactory( Renderer &renderer ) :
    mRenderer( renderer )
{
}

Ogre::Item* MeshFactory::createCube()
{
    return mRenderer.getSceneManager()->createItem(
        "ogrehead.mesh",
        Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
        Ogre::SCENE_DYNAMIC);
}
