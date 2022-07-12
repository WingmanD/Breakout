#pragma once


#include "Scene.hpp"
#include "Renderer.hpp"
#include "SoundEngine.hpp"
#include "Player.hpp"

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

class Engine {

    std::filesystem::path runtimePath;
    SoundEngine* soundEngine;

    int width = 1280, height = 720;
    glm::vec2 cursorPos = glm::vec2(0, 0);
    bool bPaused = false;

    GLFWwindow* windowOGL = nullptr;

    Scene* scene = nullptr;
    Renderer* renderer = nullptr;

    Player* activePlayer = nullptr;


public:
    Engine(std::filesystem::path runtimePath);

    void run();

    void possess(Player* player);

    [[nodiscard]] std::filesystem::path getRuntimePath() const { return runtimePath; }
    [[nodiscard]] SoundEngine* getSoundEngine() const { return soundEngine; }
    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

    [[nodiscard]] int* getWidthRef()  { return &width; }
    [[nodiscard]] int *getHeightRef()  { return &height; }
    [[nodiscard]] glm::vec2 getCursorPosition() const { return cursorPos; }
    [[nodiscard]] Scene* getScene() const { return scene; }
    [[nodiscard]] Renderer* getRenderer() const { return renderer; }
    [[nodiscard]] Player* getPossessedPlayer() const { return activePlayer; }

    void setCursorVisible(bool visible) const;
    void setPaused(bool paused);
    void setScene(Scene* newScene) { this->scene = newScene; }

    ~Engine();
private:
    void initOpenGL();
    void initUI();

    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
