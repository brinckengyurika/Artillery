#include "Scene.h"

#include "Renderer.h"
#include "MeshFactory.h"


#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreItem.h>


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
bool Scene::createObjects() {
//    Ogre::Item *cube = mMeshFactory.createCube();

    Ogre::SceneNode *node =
        mRenderer.getSceneManager()
        ->getRootSceneNode()
        ->createChildSceneNode();

    node->setPosition(0,0,0);
//    node->attachObject(cube);

    return true;
}
void Scene::shutdown() {
}
