#include "FiniteStateEngine.h"

FiniteStateEngine::FiniteStateEngine(
    NodeMetadataStorage& storage)
    : m_storage(storage)
{
}

std::size_t FiniteStateEngine::getObjectCount(
    const std::string& type) const
{
    return m_storage.size(type);
}

void FiniteStateEngine::update(float dt)
{
    // Itt fogjuk később feldolgozni
    // a JSON-ban definiált állapotátmeneteket.

    (void)dt;
}
