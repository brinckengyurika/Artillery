
#pragma once

#include <string>
#include <unordered_map>

#include <OgreMesh2.h>

class ModelCache
{
public:
    ModelCache() = default;

    // Megkeresi a már betöltött mesh-t.
    Ogre::MeshPtr getMesh(const std::string& key) const;

    // Eltárolja az újonnan elkészített mesh-t.
    void storeMesh(
        const std::string& key,
        const Ogre::MeshPtr& mesh);

    bool contains(const std::string& key) const;

    std::size_t size() const;

    void clear();

private:
    std::unordered_map<std::string, Ogre::MeshPtr> m_meshes;
};
