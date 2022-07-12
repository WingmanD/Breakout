#pragma once

#include "Object.hpp"

class Player : public Object {

public:
    Player(Engine* owner): Object(owner) {}

    virtual void onMouseMove(double xpos, double ypos) {}
    virtual void onScroll(double xoffset, double yoffset) {}
    virtual void onKey(int key, int scancode, int action, int mods) {}
};
