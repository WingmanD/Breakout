#pragma once

class Object;

class Component {
protected:
    Object* owner = nullptr;
public:
    Component() = default;

    virtual void tick() {}

    void attachComponentToObject(Object* newParent) {
        this->owner = newParent;
    }
    void detach() { this->owner = nullptr; }

    [[nodiscard]] Object* getParent() const { return owner; }

    virtual ~Component() = default;
};
