#include "SceneComponent.hpp"

#include <iostream>
#include <ostream>

#include "Object.hpp"
#include "Transform.hpp"

void SceneComponent::draw() { for (const auto child : childComponents) child->draw(); }

void SceneComponent::draw(const Transform& transform) {
    for (const auto child : childComponents) child->draw();
}

void SceneComponent::attachComponentToComponent(SceneComponent* component) {
    if (!component) {
        std::cerr << "Error: SceneComponent::attachComponentToComponent: component is nullptr" << std::endl;
        return;
    }
    attachTo(component);
    component->childComponents.emplace_back(this);
}

void SceneComponent::attachComponentToObject(Object* newOwner) {
    if (!newOwner) {
        std::cerr << "Error: SceneComponent::attachComponentToObject: newOwner is nullptr" << std::endl;
        return;
    }
    Component::attachComponentToObject(newOwner);
    attachTo(newOwner);
}
