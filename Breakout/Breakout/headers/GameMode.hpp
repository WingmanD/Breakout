#pragma once
#include "Controllable.hpp"
#include "GLFW/glfw3.h"
#include "Engine.hpp"

class GameMode : public Controllable {

protected:
    Engine* engine = nullptr;
    bool bPaused = false;
public:
    GameMode(Engine* owner): engine(owner) {}

    virtual void start() = 0;
    virtual void end() = 0;

    virtual void pause() { bPaused = true; }
    virtual void resume() { bPaused = false; }

    virtual void reset() = 0;

    virtual void quit() { glfwSetWindowShouldClose(engine->getOpenGLWindow(), GL_TRUE); }

    [[nodiscard]] bool isPaused() const { return bPaused; }

    ~GameMode() override = default;
};
