#include "NodeMetadataStorage.h"

void NodeMetadataStorage::store(
    const std::string& type,
    const std::string& name,
    NodeAndMetadata data)
{
    TypeStorage& storage = m_types[type];

    auto it = storage.nameIndex.find(name);

    if (it != storage.nameIndex.end())
    {
        storage.objects[it->second].data =
            std::move(data);
        return;
    }

    const std::size_t index =
        storage.objects.size();

    storage.objects.emplace_back(
        name,
        std::move(data)
    );

    storage.nameIndex[name] = index;
}

NodeAndMetadata* NodeMetadataStorage::get(
    const std::string& type,
    const std::string& name)
{
    auto typeIt = m_types.find(type);

    if (typeIt == m_types.end())
        return nullptr;

    TypeStorage& storage = typeIt->second;

    auto nameIt = storage.nameIndex.find(name);

    if (nameIt == storage.nameIndex.end())
        return nullptr;

    return &storage.objects[nameIt->second].data;
}

const NodeAndMetadata* NodeMetadataStorage::get(
    const std::string& type,
    const std::string& name) const
{
    auto typeIt = m_types.find(type);

    if (typeIt == m_types.end())
        return nullptr;

    const TypeStorage& storage = typeIt->second;

    auto nameIt = storage.nameIndex.find(name);

    if (nameIt == storage.nameIndex.end())
        return nullptr;

    return &storage.objects[nameIt->second].data;
}

NodeMetadataStorage::TypeStorage*
NodeMetadataStorage::getType(
    const std::string& type)
{
    auto it = m_types.find(type);

    if (it == m_types.end())
        return nullptr;

    return &it->second;
}

const NodeMetadataStorage::TypeStorage*
NodeMetadataStorage::getType(
    const std::string& type) const
{
    auto it = m_types.find(type);

    if (it == m_types.end())
        return nullptr;

    return &it->second;
}

bool NodeMetadataStorage::contains(
    const std::string& type,
    const std::string& name) const
{
    const TypeStorage* storage = getType(type);

    if (!storage)
        return false;

    return storage->nameIndex.find(name) !=
           storage->nameIndex.end();
}

bool NodeMetadataStorage::remove(
    const std::string& type,
    const std::string& name)
{
    auto typeIt = m_types.find(type);

    if (typeIt == m_types.end())
        return false;

    TypeStorage& storage = typeIt->second;

    auto nameIt = storage.nameIndex.find(name);

    if (nameIt == storage.nameIndex.end())
        return false;

    const std::size_t index = nameIt->second;
    const std::size_t last =
        storage.objects.size() - 1;

    if (index != last)
    {
        storage.objects[index] =
            std::move(storage.objects[last]);

        storage.nameIndex[
            storage.objects[index].name
        ] = index;
    }

    storage.objects.pop_back();
    storage.nameIndex.erase(nameIt);

    if (storage.objects.empty())
        m_types.erase(typeIt);

    return true;
}

std::size_t NodeMetadataStorage::size(
    const std::string& type) const
{
    const TypeStorage* storage = getType(type);

    return storage ? storage->objects.size() : 0;
}

void NodeMetadataStorage::clear()
{
    m_types.clear();
}
