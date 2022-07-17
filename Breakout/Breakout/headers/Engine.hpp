#pragma once

#include "Scene.hpp"
#include "Renderer.hpp"
#include "SoundEngine.hpp"
#include "Player.hpp"
#include "WidgetManager.hpp"

#include "GLFW/glfw3.h"

class GameMode;

/**
 *  Engine class that manages the game loop and the game mode. Initializes OpenGL and other subsystems. Handles game closing
 */
class Engine {

    std::filesystem::path runtimePath;
    SoundEngine* soundEngine;

    int width = 1280, height = 720;
    glm::vec2 cursorPos = glm::vec2(0, 0);
    bool bPaused = false;

    GLFWwindow* windowOGL = nullptr;

    Scene* scene = nullptr;
    Renderer* renderer = nullptr;
    WidgetManager* widgetManager = nullptr;

    Player* activePlayer = nullptr;

    GameMode* gameMode = nullptr;

public:
    Engine(std::filesystem::path runtimePath);

    void run();

    void possess(Player* player);

    /**
     * Get path to executable directory (where the game is run from) for relative paths for loading resources
     */
    [[nodiscard]] std::filesystem::path getRuntimePath() const { return runtimePath; }
    [[nodiscard]] SoundEngine* getSoundEngine() const { return soundEngine; }
    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

    [[nodiscard]] int* getWidthRef() { return &width; }
    [[nodiscard]] int* getHeightRef() { return &height; }
    [[nodiscard]] glm::vec2 getCursorPosition() const { return cursorPos; }
    [[nodiscard]] Scene* getScene() const { return scene; }
    [[nodiscard]] Renderer* getRenderer() const { return renderer; }
    [[nodiscard]] Player* getPossessedPlayer() const { return activePlayer; }
    [[nodiscard]] GameMode* getGameMode() const { return gameMode; }
    [[nodiscard]] WidgetManager* getWidgetManager() const { return widgetManager; }
    [[nodiscard]] GLFWwindow* getOpenGLWindow() const { return windowOGL; }

    void setCursorVisible(bool visible) const;
    void setPaused(bool paused);
    void setScene(Scene* newScene) { this->scene = newScene; }
    void setGameMode(GameMode* const newGameMode) { gameMode = newGameMode; }

    ~Engine();

private:
    void initOpenGL();
    void initUI() const;

    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
