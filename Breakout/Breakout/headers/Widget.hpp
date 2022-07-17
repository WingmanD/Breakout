#pragma once
#include <string>

#include "Drawable.hpp"
#include "imgui/imgui.h"

#include "glm/vec2.hpp"


/**
 *  Base class for ImGui widgets. Needs to be added manually to the engine's widget manager.
 */
class Widget : public Drawable {

protected:
    bool bIsVisible = false;
    std::string name;
    
    int* width;
    int* height;

    glm::vec2 position = {0, 0};

    ImGuiWindowFlags windowFlags = 0;

public:
    Widget(std::string name, int* width, int* height);
    Widget(std::string name, int width, int height);

    void init() override {}

    void draw() override {
        ImGui::SetNextWindowPos({position.x, position.y});
        ImGui::Begin(name.c_str(), nullptr, windowFlags);
        setupUI();
        ImGui::End();
    }

    void draw(const Transform& transform) override {}

    /**
     *  Called when the widget is opened. Put ImGui code here.
     */
    virtual void setupUI() = 0;

    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] bool isVisible() const { return bIsVisible; }
    [[nodiscard]] ImGuiWindowFlags getWindowFlags() const { return windowFlags; }
    [[nodiscard]] glm::vec2 getPosition() const { return position; }

    void setName(const std::string& newName) { this->name = newName; }
    void setVisibility(const bool visibility) { bIsVisible = visibility; }
    void setWindowFlags(const ImGuiWindowFlags newWindowFlags) { windowFlags = newWindowFlags; }
    void setPosition(const glm::vec2& newPosition) { this->position = newPosition; }

    ~Widget() override = default;
};
