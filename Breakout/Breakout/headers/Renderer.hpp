#pragma once

#include "Scene.hpp"
#include "glm/vec4.hpp"

#include <glad/glad.h>

struct LightDataEntry {
    alignas(16) glm::vec4 position;
    alignas(16) glm::vec4 color;
    float intensity;
};

class Renderer {
    Scene* scene;
    
    GLuint UBO_ViewData{};
    GLuint SSBO_LightData{};

public:
    Renderer(Scene* scene);

    void render() const;

    inline void initViewData();
    inline void updateViewData() const;

    inline void initLightData();
    inline void updateLightData() const;
    
    [[nodiscard]] Scene* getScene() const { return scene; }
    void setScene(Scene* newScene) { this->scene = newScene; }

    ~Renderer();
};
