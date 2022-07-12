#pragma once
#include "SceneComponent.hpp"
#include "StaticMesh.hpp"

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
    
    ~StaticMeshComponent() override;
};
