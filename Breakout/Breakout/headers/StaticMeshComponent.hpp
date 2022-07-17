#pragma once

#include "SceneComponent.hpp"
#include "StaticMesh.hpp"

/**
 *  Component that instances a StaticMesh. Used for adding a static mesh to an object.
 */
class StaticMeshComponent : public SceneComponent {
    StaticMesh* mesh;
public:
    explicit StaticMeshComponent(StaticMesh* const mesh)
        : mesh(mesh) {
    }
    
    [[nodiscard]] StaticMesh* getMesh() const { return mesh; }
    void setMesh(StaticMesh* newMesh) { this->mesh = newMesh; }

    void init() override {}
    void draw() override;
    void draw(const Transform& transform) override;
    void tick() override;
};
