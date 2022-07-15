#include "SceneComponent.hpp"


#include "Object.hpp"
#include "Transform.hpp"

void SceneComponent::draw() {
    for (const auto child : children)
        if (const auto drawable = dynamic_cast<Drawable*>(child))
            drawable->draw();
}

void SceneComponent::draw(const Transform& transform) { draw(); }


