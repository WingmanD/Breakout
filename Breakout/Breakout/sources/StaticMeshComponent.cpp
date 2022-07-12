#include "StaticMeshComponent.hpp"

void StaticMeshComponent::draw() { draw(static_cast<Transform>(this)); }


void StaticMeshComponent::draw(const Transform& transform) {
    if (mesh) mesh->draw(transform);
}

void StaticMeshComponent::tick() { SceneComponent::tick(); }

StaticMeshComponent::~StaticMeshComponent() {
    // todo
    // delete mesh;
}
