#pragma once

#include <array>
#include <string>
#include <cstddef>
#include <OgreSceneNode.h>
#include <array>
#include <unordered_map>


constexpr std::size_t STATUSLIST = 10;

class NodeAndMetadata
{
public:
//    NodeAndMetadata() = default;
    NodeAndMetadata(const std::string& type, const std::string& name, Ogre::SceneNode* node, unsigned int status);
    ~NodeAndMetadata();
    void Setcurrentstatus(unsigned int val);
    unsigned int Stepbackstatus();

    Ogre::SceneNode* node = nullptr;

    std::array<std::string, STATUSLIST> strings{};
    std::array<int, STATUSLIST> statuses{};
    std::unordered_map<std::string, float> tmpvars;

    private:
        std::string type;
        std::string name;
};
