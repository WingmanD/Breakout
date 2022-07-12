#pragma once

class Object;

class Component {
protected:
    Object* parent = nullptr;
public:
    Component() = default;

    virtual void tick() {}

    void attachComponentToObject(Object* newParent) { this->parent = newParent; }
    void detach() { this->parent = nullptr; }

    [[nodiscard]] Object* getParent() const { return parent; }

    virtual ~Component() = default;
};
