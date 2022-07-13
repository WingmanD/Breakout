#pragma once

#include <vector>

#include "Component.hpp"
#include "Drawable.hpp"
#include "Transform.hpp"

class SceneComponent : public Component, public Transform, public Drawable {
    std::vector<SceneComponent*> childComponents;
public:
    SceneComponent() = default;

    void attachComponentToComponent(SceneComponent* component) {
        this->parent = component;
        component->childComponents.emplace_back(this);
    }

};
