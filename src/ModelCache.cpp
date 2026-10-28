
#include "ModelCache.h"

Ogre::MeshPtr ModelCache::getMesh(
    const std::string& key) const
{
    const auto it = m_meshes.find(key);

    if (it == m_meshes.end())
        return Ogre::MeshPtr();

    return it->second;
}

void ModelCache::storeMesh(
    const std::string& key,
    const Ogre::MeshPtr& mesh)
{
    if (mesh.isNull())
        return;

    m_meshes[key] = mesh;
}

bool ModelCache::contains(
    const std::string& key) const
{
    return m_meshes.find(key) != m_meshes.end();
}

std::size_t ModelCache::size() const
{
    return m_meshes.size();
}

void ModelCache::clear()
{
    m_meshes.clear();
}
