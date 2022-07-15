#include "Object.hpp"

void Object::draw() {
    for (const auto child : children) {
        if (const auto drawable = dynamic_cast<Drawable*>(child))
            drawable->draw();
    }
}
