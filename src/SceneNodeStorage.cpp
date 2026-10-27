#include "SceneNodeStorage.h"

void SceneNodeStorage::storeNode(
    const std::string& name,
    Ogre::SceneNode* node)
{
    if (!node)
        return;

    auto it = m_index.find(name);

    if (it != m_index.end())
    {
        m_nodes[it->second].node = node;
        return;
    }

    m_index[name] = m_nodes.size();
    m_nodes.push_back({name, node});
}

Ogre::SceneNode* SceneNodeStorage::getNode(
    const std::string& name) const
{
    auto it = m_index.find(name);

    if (it == m_index.end())
        return nullptr;

    return m_nodes[it->second].node;
}

bool SceneNodeStorage::contains(
    const std::string& name) const
{
    return m_index.find(name) != m_index.end();
}

bool SceneNodeStorage::removeNode(
    const std::string& name)
{
    auto it = m_index.find(name);

    if (it == m_index.end())
        return false;

    const std::size_t index = it->second;
    const std::size_t last = m_nodes.size() - 1;

    // Az utolsó elemet a törölt elem helyére mozgatjuk.
    if (index != last)
    {
        m_nodes[index] = std::move(m_nodes[last]);
        m_index[m_nodes[index].name] = index;
    }

    m_nodes.pop_back();
    m_index.erase(it);

    return true;
}



std::size_t SceneNodeStorage::size() const
{
    return m_nodes.size();
}
