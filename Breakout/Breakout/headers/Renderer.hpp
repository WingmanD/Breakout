#pragma once

#include "Camera.hpp"
#include "Scene.hpp"

class Renderer {
    Camera* camera;
    Scene* scene;
    GLuint UBO_ViewData{};

public:
    Renderer(Scene* scene, Camera* camera);

    void render() const;

    inline void initViewData();
    inline void updateViewData() const;

    ~Renderer();
};
