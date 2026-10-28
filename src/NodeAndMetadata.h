#pragma once

#include <array>
#include <string>
#include <cstddef>
#include <OgreSceneNode.h>

constexpr std::size_t STATUSLIST = 10;

class NodeAndMetadata
{
public:
    NodeAndMetadata() = default;

    Ogre::SceneNode* node = nullptr;

    std::array<std::string, STATUSLIST> strings{};
    std::array<int, STATUSLIST> statuses{};
};
