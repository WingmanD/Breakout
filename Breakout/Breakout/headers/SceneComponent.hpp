#pragma once

#include "Component.hpp"
#include "Drawable.hpp"
#include "Transform.hpp"

/**
 *  A component that can be drawn and moved in the scene. Can be used as empty sub-component or carry a module (for instance, StaticMesh).
 */
class SceneComponent : public Component, public Transform, public Drawable {
public:
    SceneComponent() = default;

    void tick() override {}

    void draw() override;
    void draw(const Transform& transform) override;
};
