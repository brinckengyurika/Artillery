
#pragma once

#include <string>

namespace tinygltf
{
    class Model;
}

class Renderer;

class GltfLoader
{
public:
    explicit GltfLoader(Renderer &renderer);

    bool load(const std::string &filename);

private:
    Renderer &mRenderer;
};
