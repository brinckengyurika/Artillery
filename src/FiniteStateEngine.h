#ifndef FINITESTATEENGINE_H
#define FINITESTATEENGINE_H

#include "NodeMetadataStorage.h"

#include <cstddef>
#include <string>

class FiniteStateEngine
{
private:
    NodeMetadataStorage& m_storage;

public:
    explicit FiniteStateEngine(NodeMetadataStorage& storage);

    std::size_t getObjectCount(const std::string& type) const;

    void update(float dt);
};

#endif // FINITESTATEENGINE_H
