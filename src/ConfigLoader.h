#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <vector>
#include <array>
#include <nlohmann/json.hpp>

// Konfigurációs adatstruktúrák
class WindowConfig {
public:
    int width{0};
    int height{0};
    bool fullscreen{false};
    bool vsync{false};
};

class CameraConfig {
public:
    std::string name;
    std::array<float, 3> position{0.0f, 0.0f, 0.0f};
    std::array<float, 3> look_at{0.0f, 0.0f, 0.0f};
    float near_clip{0.0f};
    float far_clip{0.0f};
};

class TerrainConfig {
public:
    std::string name;
    std::string heightmap;
    std::array<float, 3> position{0.0f, 0.0f, 0.0f};
    std::array<float, 3> size{0.0f, 0.0f, 0.0f};
};

class SceneConfig {
public:
    std::string model;
};

class Config {
public:
    WindowConfig window;
    std::vector<CameraConfig> cameras;
    std::vector<TerrainConfig> terrains;
    SceneConfig scene;
};

// nlohmann::json konverziós deklarációk
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowConfig, width, height, fullscreen, vsync)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraConfig, name, position, look_at, near_clip, far_clip)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TerrainConfig, name, heightmap, position, size)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SceneConfig, model)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, window, cameras, terrains, scene)

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
