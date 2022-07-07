#pragma once

#include <vector>

#include "Object.hpp"

class Scene {
public:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    
    Camera* activeCamera;

    Scene(Camera* camera) : activeCamera(camera) {}

    void tick(double deltaTime) const;

    Object* load(std::filesystem::path path, Shader* shader, bool bLoadMaterials = true);

    void addObject(Object* object);
    void addLight(Light* light);

    ~Scene();
};
