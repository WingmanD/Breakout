#pragma once

#include "Scene.hpp"
#include "glm/vec4.hpp"

#include <glad/glad.h>

struct LightDataEntry {
    alignas(16) glm::vec4 position;
    alignas(16) glm::vec4 color;
    float intensity;
};

/**
 * Class that renders a scene with a variable number of point lights.
 * 
 */
class Renderer {
    Scene* scene;

    bool unlit = false;

    GLuint UBO_ViewData{};
    GLuint SSBO_LightData{};

    inline void initViewData();
    inline void updateViewData() const;

    inline void initLightData();
    inline void updateLightData() const;

public:
    Renderer(Scene* scene);

    void render() const;
    
    [[nodiscard]] bool isUnlit() const { return unlit; }
    [[nodiscard]] Scene* getScene() const { return scene; }

    void setUnlit(const bool status) { this->unlit = status; }
    void setScene(Scene* newScene) { this->scene = newScene; }

    ~Renderer();
};
