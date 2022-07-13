#include "StaticMeshComponent.hpp"

void StaticMeshComponent::draw() {
    SceneComponent::draw();
    draw(*this);
}


void StaticMeshComponent::draw(const Transform& transform) {
    SceneComponent::draw(transform);

    if (mesh) mesh->draw(transform);
}

void StaticMeshComponent::tick() { SceneComponent::tick(); }

StaticMeshComponent::~StaticMeshComponent() {
    // todo
    // delete mesh;
}
