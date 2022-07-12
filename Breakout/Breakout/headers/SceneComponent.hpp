#pragma once

#include "Component.hpp"
#include "Drawable.hpp"
#include "Transform.hpp"

class SceneComponent : public Component, public Transform, public Drawable {
public:
    SceneComponent() = default;
};
