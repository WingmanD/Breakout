#pragma once

#include <vector>

#include "Component.hpp"
#include "Drawable.hpp"
#include "Transform.hpp"

class SceneComponent : public Component, public Transform, public Drawable {
    std::vector<SceneComponent*> childComponents;
public:
    SceneComponent() = default;

    void draw() override;
    void draw(const Transform& transform) override;

    void attachComponentToComponent(SceneComponent* component);

    void attachComponentToObject(Object* newOwner) override;
};
