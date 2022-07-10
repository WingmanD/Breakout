#pragma once

#include "Camera.hpp"
#include "Scene.hpp"

struct LightDataEntry {
    alignas(16) glm::vec4 position;
    alignas(16) glm::vec4 color;
    float intensity;
};

class Renderer {
    Camera* camera;
    Scene* scene;
    
    GLuint UBO_ViewData{};
    GLuint SSBO_LightData{};

public:
    Renderer(Scene* scene, Camera* camera);

    void render() const;

    inline void initViewData();
    inline void updateViewData() const;

    inline void initLightData();
    inline void updateLightData() const;
    
    ~Renderer();
};
