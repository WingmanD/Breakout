#pragma once
#include "GameMode.hpp"
#include "Widget.hpp"

class BreakoutBoard;

class BreakoutGameMode : public GameMode {
    int initialLives = 5;
    
    int currentLives = 5;
    int score = 0;
    int level = 0;

    Widget* mainMenuWidget = nullptr;
    Widget* pauseMenuWidget = nullptr;
    Widget* scoreWidget = nullptr;

    BreakoutBoard* board = nullptr;
    
    bool bInMainMenu = true;
public:
    explicit BreakoutGameMode(Engine* owner, int lives);

    void updateScore(int points);
    void lostBall();

    void loadNextLevel();

    void start() override;
    void end() override;
    void pause() override;
    void reset() override;
    void resume() override;

    void onKey(int key, int scancode, int action, int mods) override;

    [[nodiscard]] int getLivesCount() const { return currentLives; }
    [[nodiscard]] int getScore() const { return score; }
    [[nodiscard]] int getLevel() const { return level; }
};
