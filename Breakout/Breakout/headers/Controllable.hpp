#pragma once

/**
 * Interface for objects that can take controls of the game.
 *
 */
class Controllable {
public:
    virtual void onMouseMove(double xpos, double ypos) {}
    virtual void onScroll(double xoffset, double yoffset) {}
    virtual void onKey(int key, int scancode, int action, int mods) {}
    virtual void onWindowSizeChange(int width, int height) {}

    virtual ~Controllable() = default;
};
