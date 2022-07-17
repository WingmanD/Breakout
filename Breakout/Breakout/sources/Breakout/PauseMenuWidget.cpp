#include "Breakout/PauseMenuWidget.hpp"

#include "Breakout/BreakoutGameMode.hpp"

PauseMenuWidget::PauseMenuWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight): Widget("Main Menu",
    windowWidth,
    windowHeight), gameMode(mode) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void PauseMenuWidget::setupUI() {
    auto size = ImGui::CalcTextSize("MAIN MENU");
    position = {(static_cast<float>(*width) - size.x) / 2, (static_cast<float>(*height) - size.y * 4) / 2};

    ImGui::SetWindowSize({size.x * 1.5f, size.y * 6});

    ImGui::Text("PAUSED");
    ImGui::Separator();
    if (ImGui::Button("RESUME")) { gameMode->resume(); }
    if (ImGui::Button("MAIN MENU")) { gameMode->openMainMenu(); }
}
