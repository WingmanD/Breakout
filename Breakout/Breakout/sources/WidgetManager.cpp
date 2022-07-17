#include "WidgetManager.hpp"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void WidgetManager::drawWidgets() const {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (const auto widget : widgets) if (widget->isVisible()) widget->draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WidgetManager::addWidget(Widget* widget) { widgets.emplace_back(widget); }

void WidgetManager::removeWidget(Widget* widget) { std::erase(widgets, widget); }

WidgetManager::~WidgetManager() { for (const auto widget : widgets) delete widget; }
