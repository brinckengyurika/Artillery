#include "ConfigLoader.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

ConfigLoader::ConfigLoader(const std::string& filename) {
    std::cout << "Filename: " << filename << std::endl;
    loadFromFile(filename);
}

void ConfigLoader::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Hiba: Nem sikerült megnyitni a " << filename << " fájlt!" << std::endl;
        m_isLoaded = false;
        return;
    }

    try {
        json j;
        file >> j;
        m_config = j.get<Config>();
        m_isLoaded = true;
    } catch (const json::exception& e) {
        std::cerr << "JSON parsolási hiba: " << e.what() << std::endl;
        m_isLoaded = false;
    }
}

const Config& ConfigLoader::getConfig() const {
    return m_config;
}

bool ConfigLoader::isLoaded() const {
    return m_isLoaded;
}
