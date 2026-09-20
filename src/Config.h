#pragma once
#pragma once

#include <string>
#include <vector>
#include <array>
#include <nlohmann/json.hpp>

using Vec3 = std::array<float, 3>;

// --- 1. LIGHT CLASS ---
class LightConfig {
private:
    std::string m_name;
    Vec3 m_position{0.0f, 0.0f, 0.0f};
    Vec3 m_direction{0.0f, 0.0f, 0.0f};
    std::string m_type;
    Vec3 m_color{255.0f, 255.0f, 255.0f};
    Vec3 m_specularcolor{1.0f, 1.0f, 1.0f};

public:
    LightConfig() = default;

    const std::string& getName() const { return m_name; }
    const Vec3& getPosition() const { return m_position; }
    const Vec3& getDirection() const { return m_direction; }
    const std::string& getType() const { return m_type; }
    const Vec3& getColor() const { return m_color; }
    const Vec3& getSpecularColor() const { return m_specularcolor; }

    friend void from_json(const nlohmann::json& j, LightConfig& item) {
        j.at("name").get_to(item.m_name);
        j.at("position").get_to(item.m_position);
        j.at("direction").get_to(item.m_direction);
        j.at("type").get_to(item.m_type);
        j.at("color").get_to(item.m_color);
        j.at("specularcolor").get_to(item.m_specularcolor);
    }
};

// --- 2. CAMERA CLASS ---
class CameraConfig {
private:
    std::string m_name;
    Vec3 m_position{0.0f, 0.0f, 0.0f};
    Vec3 m_lookAt{0.0f, 0.0f, 0.0f};
    float m_nearClip{0.1f};
    float m_farClip{1000.0f};

public:
    CameraConfig() = default;

    const std::string& getName() const { return m_name; }
    const Vec3& getPosition() const { return m_position; }
    const Vec3& getLookAt() const { return m_lookAt; }
    float getNearClip() const { return m_nearClip; }
    float getFarClip() const { return m_farClip; }

    friend void from_json(const nlohmann::json& j, CameraConfig& item) {
        j.at("name").get_to(item.m_name);
        j.at("position").get_to(item.m_position);
        j.at("look_at").get_to(item.m_lookAt);
        j.at("near_clip").get_to(item.m_nearClip);
        j.at("far_clip").get_to(item.m_farClip);
    }
};

// --- 3. TERRAIN CLASS ---
class TerrainConfig {
private:
    std::string m_name;
    std::string m_heightmap;
    Vec3 m_position{0.0f, 0.0f, 0.0f};
    Vec3 m_size{1.0f, 1.0f, 1.0f};

public:
    TerrainConfig() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getHeightmap() const { return m_heightmap; }
    const Vec3& getPosition() const { return m_position; }
    const Vec3& getSize() const { return m_size; }

    friend void from_json(const nlohmann::json& j, TerrainConfig& item) {
        j.at("name").get_to(item.m_name);
        j.at("heightmap").get_to(item.m_heightmap);
        j.at("position").get_to(item.m_position);
        j.at("size").get_to(item.m_size);
    }
};

// --- 4. MODEL ASSET CLASS ---
class ModelAssetConfig {
private:
    std::string m_name;
    std::string m_modelPath;

public:
    ModelAssetConfig() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getModelPath() const { return m_modelPath; }

    friend void from_json(const nlohmann::json& j, ModelAssetConfig& item) {
        j.at("name").get_to(item.m_name);
        j.at("model").get_to(item.m_modelPath);
    }
};

// --- 5. SCENE INSTANCE CLASSES ---
class SceneModelInstance {
private:
    std::string m_name;
    std::string m_modelName;
    Vec3 m_scale{1.0f, 1.0f, 1.0f};
    Vec3 m_position{0.0f, 0.0f, 0.0f};
    Vec3 m_rotation{0.0f, 0.0f, 0.0f};

public:
    SceneModelInstance() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getModelName() const { return m_modelName; }
    const Vec3& getScale() const { return m_scale; }
    const Vec3& getPosition() const { return m_position; }
    const Vec3& getRotation() const { return m_rotation; }

    friend void from_json(const nlohmann::json& j, SceneModelInstance& item) {
        j.at("name").get_to(item.m_name);
        j.at("modelname").get_to(item.m_modelName);
        j.at("scale").get_to(item.m_scale);
        j.at("position").get_to(item.m_position);
        j.at("rotation").get_to(item.m_rotation);
    }
};

class SceneCameraInstance {
private:
    std::string m_name;
    std::string m_cameraName;

public:
    SceneCameraInstance() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getCameraName() const { return m_cameraName; }

    friend void from_json(const nlohmann::json& j, SceneCameraInstance& item) {
        j.at("name").get_to(item.m_name);
        j.at("cameraname").get_to(item.m_cameraName);
    }
};

class SceneLightInstance {
private:
    std::string m_name;
    std::string m_lightName;

public:
    SceneLightInstance() = default;

    const std::string& getName() const { return m_name; }
    const std::string& getLightName() const { return m_lightName; }

    friend void from_json(const nlohmann::json& j, SceneLightInstance& item) {
        j.at("name").get_to(item.m_name);
        j.at("lightname").get_to(item.m_lightName);
    }
};

// --- 6. SCENE CLASS ---
class SceneConfig {
private:
    std::string m_name;
    std::vector<SceneModelInstance> m_models;
    std::vector<SceneCameraInstance> m_cameras;
    std::vector<SceneLightInstance> m_lights;
    std::string m_terrain;

public:
    SceneConfig() = default;

    const std::string& getName() const { return m_name; }
    const std::vector<SceneModelInstance>& getModels() const { return m_models; }
    const std::vector<SceneCameraInstance>& getCameras() const { return m_cameras; }
    const std::vector<SceneLightInstance>& getLights() const { return m_lights; }
    const std::string& getTerrain() const { return m_terrain; }

    friend void from_json(const nlohmann::json& j, SceneConfig& item) {
        j.at("name").get_to(item.m_name);
        j.at("models").get_to(item.m_models);
        j.at("cameras").get_to(item.m_cameras);
        j.at("lights").get_to(item.m_lights);
        j.at("terrain").get_to(item.m_terrain);
    }
};

// --- 7. WINDOW CONFIG CLASS ---
class WindowConfig {
private:
    std::string m_resolution;
    std::string m_vsync;
    std::string m_fullscreen;

public:
    WindowConfig() = default;

    const std::string& getResolution() const { return m_resolution; }
    bool isVsyncEnabled() const { return m_vsync == "Yes" || m_vsync == "true"; }
    bool isFullscreen() const { return m_fullscreen == "Yes" || m_fullscreen == "true"; }

    friend void from_json(const nlohmann::json& j, WindowConfig& item) {
        j.at("resolution").get_to(item.m_resolution);
        j.at("vsync").get_to(item.m_vsync);
        j.at("fullscreen").get_to(item.m_fullscreen);
    }
};

// --- 8. ROOT APP CONFIG CLASS ---
class Config {
private:
    std::vector<LightConfig> m_lights;
    std::vector<CameraConfig> m_cameras;
    std::vector<TerrainConfig> m_terrains;
    std::vector<ModelAssetConfig> m_models;
    std::vector<SceneConfig> m_scenes;
    WindowConfig m_window;

public:
    Config() = default;

    const std::vector<LightConfig>& getLights() const { return m_lights; }
    const std::vector<CameraConfig>& getCameras() const { return m_cameras; }
    const std::vector<TerrainConfig>& getTerrains() const { return m_terrains; }
    const std::vector<ModelAssetConfig>& getModels() const { return m_models; }
    const std::vector<SceneConfig>& getScenes() const { return m_scenes; }
    const WindowConfig& getWindow() const { return m_window; }

    friend void from_json(const nlohmann::json& j, Config& item) {
        j.at("lights").get_to(item.m_lights);
        j.at("cameras").get_to(item.m_cameras);
        j.at("terrains").get_to(item.m_terrains);
        j.at("models").get_to(item.m_models);
        j.at("scenes").get_to(item.m_scenes);
        j.at("window").get_to(item.m_window);
    }
};
