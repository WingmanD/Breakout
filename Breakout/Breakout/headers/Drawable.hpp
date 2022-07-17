#pragma once

#include "Transform.hpp"

/**
 *  Interface for all objects that can be rendered.
 * 
 */
class Drawable {
public:
  
    virtual void init() = 0;
    
    virtual void draw() = 0;
    virtual void draw(const Transform& transform) = 0;

    virtual ~Drawable() = default;
};
