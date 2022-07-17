#include "StaticMeshComponent.hpp"

void StaticMeshComponent::draw() {
    SceneComponent::draw();
    draw(*this);
}


void StaticMeshComponent::draw(const Transform& transform) { if (mesh) mesh->draw(transform); }

void StaticMeshComponent::tick() { SceneComponent::tick(); }
