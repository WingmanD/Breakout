#pragma once

class Component {
public:
    Component() = default;

    virtual void tick() {}

    virtual ~Component() = default;
};
