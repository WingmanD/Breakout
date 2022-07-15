#pragma once

#include "Transform.hpp"
#include "Drawable.hpp"

class Engine;
class Component;

class Object : public Transform, public Drawable {
protected:
    Engine* engine;
    
public:
    Object(Engine* owner): engine(owner) {}

    virtual void tick(double deltaTime) {}

    void init() override {}
    void draw() override;
    void draw(const Transform& transform) override {}

    virtual ~Object() {
        Transform::destroy();
    }
};
