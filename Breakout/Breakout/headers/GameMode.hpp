#pragma once
#include "Controllable.hpp"
#include "GLFW/glfw3.h"
#include "Engine.hpp"

/**
 * Base class for game modes. Game mode takes mouse and keyboard inputs to handle widgets, handles game start, pause etc.
 */
class GameMode : public Controllable {

protected:
    Engine* engine = nullptr;
    bool bPaused = false;
public:
    GameMode(Engine* owner): engine(owner) {}

    /**
     * Called when game mode is entered.
     * 
     */
    virtual void start() = 0;

    /**
     *  Called when game is over.
     * 
     */
    virtual void end() = 0;

    /**
     * Pause game.
     * 
     */
    virtual void pause() { bPaused = true; }

    /**
     * Resume game.
     * 
     */
    virtual void resume() { bPaused = false; }

    /**
     * Reset game.
     */
    virtual void reset() = 0;

    /**
     * Quit game to desktop.
     */
    virtual void quit() { glfwSetWindowShouldClose(engine->getOpenGLWindow(), GL_TRUE); }

    [[nodiscard]] bool isPaused() const { return bPaused; }

    ~GameMode() override = default;
};
