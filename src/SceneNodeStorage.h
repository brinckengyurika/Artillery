#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <OgreSceneNode.h>

class SceneNodeStorage
{
public:
    struct Entry    {        std::string name;        Ogre::SceneNode* node;    };

    void storeNode( const std::string& name,      Ogre::SceneNode* node);
    Ogre::SceneNode* getNode( const std::string& name) const;
    bool contains(const std::string& name) const;
    bool removeNode(const std::string& name);
    std::size_t size() const;
    // Gyors iterálás
    const std::vector<Entry>& getNodes() const { return m_nodes;  }


private:
    std::vector<Entry> m_nodes;
    std::unordered_map< std::string, std::size_t > m_index;

};
