#pragma once

class Object;

class Component {
protected:
    Object* owner = nullptr;
public:
    Component() = default;

    virtual void tick() {}

    virtual void attachComponentToObject(Object* newOwner) {
        this->owner = newOwner;
    }
    
    void detachFromComponent() { this->owner = nullptr; }

    [[nodiscard]] Object* getOwner() const { return owner; }

    virtual ~Component() = default;
};
