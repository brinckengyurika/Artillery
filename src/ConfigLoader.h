#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <vector>
#include <array>
#include <nlohmann/json.hpp>
#include "Config.h"

// Konfigurációs adatstruktúrák




// Fő betöltő osztály
class ConfigLoader {
public:
    // A konstruktor megkapja a fájlnevet és automatikusan betölti
    explicit ConfigLoader(const std::string& filename);

    // Referenciát ad a belső Config objektumra
    const Config& getConfig() const;

    // Ellenőrizhető, hogy sikeres volt-e a betöltés
    bool isLoaded() const;

private:
    Config m_config;
    bool m_isLoaded{false};

    void loadFromFile(const std::string& filename);
};

#endif // CONFIG_LOADER_H
