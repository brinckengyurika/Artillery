
#pragma once

#include <string>
#include <unordered_map>

#include <OgreMesh2.h>
#include "SceneNodeStorage.h"
#include <OgreSceneNode.h>
#include "NodeMetadataStorage.h"
#include "NodeAndMetadata.h"
#include "FiniteStateEngine.h"

class ModelCache
{
public:
//    ModelCache() = default;

    ModelCache()
        : m_meshes(),
          scene_nodes(),
          storage(),
          not_static_storage(),
          m_fsm(not_static_storage) {
    }

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

    void storage_store(                 const std::string& type, const std::string& name, NodeAndMetadata data) {return storage.store(type, name, data);};
    NodeAndMetadata* storage_get(       const std::string& type, const std::string& name) { return storage.get(type, name);};
    const NodeAndMetadata* storage_get( const std::string& type, const std::string& name) const {return storage.get(type, name);};
    NodeMetadataStorage::TypeStorage* storage_getType(       const std::string& type) {return storage.getType(type);};
    const NodeMetadataStorage::TypeStorage* storage_getType( const std::string& type) const {return storage.getType(type);};
    bool storage_remove(                const std::string& type, const std::string& name) {return storage.remove(type, name);};
    bool storage_contains(              const std::string& type, const std::string& name) const {return storage.contains(type, name);};
    std::size_t storage_size(           const std::string& type) const {return storage.size(type);};
    void storage_clear()                                               { return storage.clear();};


    void not_static_storage_store(                 const std::string& type, const std::string& name, NodeAndMetadata data) {return not_static_storage.store(type, name, data);};
    NodeAndMetadata* not_static_storage_get(       const std::string& type, const std::string& name) { return not_static_storage.get(type, name);};
    const NodeAndMetadata* not_static_storage_get( const std::string& type, const std::string& name) const {return not_static_storage.get(type, name);};
    NodeMetadataStorage::TypeStorage* not_static_storage_getType(       const std::string& type) {return not_static_storage.getType(type);};
    const NodeMetadataStorage::TypeStorage* not_static_storage_getType( const std::string& type) const {return not_static_storage.getType(type);};
    bool not_static_storage_remove(                const std::string& type, const std::string& name) {return not_static_storage.remove(type, name);};
    bool not_static_storage_contains(              const std::string& type, const std::string& name) const {return not_static_storage.contains(type, name);};
    std::size_t not_static_storage_size(           const std::string& type) const {return not_static_storage.size(type);};
    void not_static_storage_clear()                                               { return not_static_storage.clear();};


private:
    std::unordered_map<std::string, Ogre::MeshPtr> m_meshes;
    std::unordered_map<std::string, SceneNodeStorage> scene_nodes;
    NodeMetadataStorage storage;
    NodeMetadataStorage not_static_storage;
    FiniteStateEngine m_fsm;
};
