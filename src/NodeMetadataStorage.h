
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <utility>

#include "NodeAndMetadata.h"

class NodeMetadataStorage
{
public:
    class Entry
    {
    public:
        std::string name;
        NodeAndMetadata data;

        Entry(
            const std::string& name,
            NodeAndMetadata data)
            : name(name),
              data(std::move(data))
        {
        }
    };

    class TypeStorage
    {
    public:
        std::vector<Entry> objects;

        std::unordered_map<
            std::string,
            std::size_t
        > nameIndex;
    };

    void store(
        const std::string& type,
        const std::string& name,
        NodeAndMetadata data);

    NodeAndMetadata* get(
        const std::string& type,
        const std::string& name);

    const NodeAndMetadata* get(
        const std::string& type,
        const std::string& name) const;

    TypeStorage* getType(
        const std::string& type);

    const TypeStorage* getType(
        const std::string& type) const;

    bool remove(
        const std::string& type,
        const std::string& name);

    bool contains(
        const std::string& type,
        const std::string& name) const;

    std::size_t size(
        const std::string& type) const;

    void clear();

private:
    std::unordered_map<
        std::string,
        TypeStorage
    > m_types;
};
