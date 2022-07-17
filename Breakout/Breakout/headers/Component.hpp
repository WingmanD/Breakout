#pragma once


/**
 * Base class for components that can be attached to objects as modules.
 */
class Component {
public:
    Component() = default;

    virtual void tick() {}

    virtual ~Component() = default;
};
