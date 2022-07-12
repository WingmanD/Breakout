#pragma once

#include <vector>

#include "Component.hpp"
#include "Drawable.hpp"
#include "SceneComponent.hpp"
#include "Transform.hpp"

class Engine;

class Object : public Transform, public Drawable {
protected:
    Engine* engine;

    std::vector<SceneComponent*> childSceneComponents;
    std::vector<Component*> childLogicComponents;
public:
    Object(Engine* owner): engine(owner) {}

    virtual void tick(double deltaTime) {}

    void init() override {}
    void draw() override;
    void draw(const Transform& transform) override {}

    void addSceneComponent(SceneComponent* component) {
        childSceneComponents.emplace_back(component);
        component->attachComponentToObject(this);
    }

    void addLogicComponent(Component* component) {
        childLogicComponents.emplace_back(component);
        component->attachComponentToObject(this);
    }

    void removeComponent(Component* component) {
        component->detach();
        std::erase(childSceneComponents, component);
        std::erase(childLogicComponents, component);
    }

    virtual ~Object() {
        delete[] childSceneComponents.data();
        delete[] childLogicComponents.data();
    }
};
