
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
    bool load(const std::string &filename, tinygltf::Model &model);

private:
    Renderer &mRenderer;
};
