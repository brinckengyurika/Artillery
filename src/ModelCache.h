
#pragma once

#include <string>
#include <unordered_map>

#include <OgreMesh2.h>
#include "SceneNodeStorage.h"
#include <OgreSceneNode.h>
#include "NodeMetadataStorage.h"

class ModelCache
{
public:
    ModelCache() = default;
    NodeMetadataStorage storage;



    struct Entry
    {
        std::string name;
        Ogre::SceneNode* node;
    };

    // Megkeresi a már betöltött mesh-t.
    Ogre::MeshPtr getMesh(const std::string& key) const;

    // Eltárolja az újonnan elkészített mesh-t.
    void storeMesh(
        const std::string& key,
        const Ogre::MeshPtr& mesh);
    bool contains(const std::string& key) const;
    std::size_t size() const;
    void clear();


    void storeNode( const std::string& type, const std::string& name,      Ogre::SceneNode* node);
    Ogre::SceneNode* getNode( const std::string& type, const std::string& name) const;
    bool contains(const std::string& type, const std::string& name) const;
    bool removeNode(const std::string& type, const std::string& name);
    std::size_t scene_node_size(const std::string& type ) const;
    // Gyors iterálás
    const std::vector<Entry>& getNodes(const std::string& type);


private:
    std::unordered_map<std::string, Ogre::MeshPtr> m_meshes;
    std::unordered_map<std::string, SceneNodeStorage> scene_nodes;
};
