#pragma once

#include <functional>

#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "StaticMesh.hpp"
#include "Shader.hpp"
#include "Transform.hpp"

class Object : public Transform {
protected:
    Shader* shader;
public:
    StaticMesh* mesh;
    Material* material;
    
    bool bHasCollision = true;

    Object(StaticMesh* mesh, Material* material, Shader* shader)
        : shader(shader), mesh(mesh), material(material) {}

    std::function<void(double)> tickFunction;
    virtual void tick(double deltaTime) { if (tickFunction) tickFunction(deltaTime); }

    virtual void drawFirstPass(Camera* camera, std::vector<Light*> &lights) { drawFirstPass(camera, static_cast<Transform>(*this),  lights); }
    virtual void drawFirstPass(Camera* camera, Transform instanceTransform, std::vector<Light*> &lights);

    virtual void drawSecondPass(Camera* camera, std::vector<Light*> &lights) { drawSecondPass(camera, static_cast<Transform>(*this),  lights); }
    virtual void drawSecondPass(Camera* camera, Transform instanceTransform, std::vector<Light*> &lights) { }

    virtual ~Object() {
        delete mesh;
        delete shader;
    }
};
