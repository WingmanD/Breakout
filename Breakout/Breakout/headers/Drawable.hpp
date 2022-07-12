#pragma once

#include "Transform.hpp"

class Drawable {
public:
    virtual void init() = 0;
    
    virtual void draw() = 0;
    virtual void draw(const Transform& transform) = 0;
};
