﻿#pragma once

#include "Controllable.hpp"
#include "Object.hpp"

class Player : public Object, public Controllable {

public:
    Player(Engine* owner): Object(owner) {}

    void onMouseMove(double xpos, double ypos) override {}
    void onScroll(double xoffset, double yoffset) override {}
    void onKey(int key, int scancode, int action, int mods) override {}
};
