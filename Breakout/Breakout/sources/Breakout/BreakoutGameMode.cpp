#include "Breakout/BreakoutGameMode.hpp"

#include <iostream>
#include <future>

#include "Breakout/BreakoutScoreWidget.hpp"
#include "Engine.hpp"
#include "Breakout/BreakoutBoard.hpp"
#include "Breakout/GameOverWidget.hpp"
#include "Breakout/MainMenuWidget.hpp"
#include "Breakout/PauseMenuWidget.hpp"
#include "Breakout/WonWidget.hpp"

BreakoutGameMode::BreakoutGameMode(Engine* owner, int lives) : GameMode(owner), initialLives(lives) {
    currentLives = lives - 1;

    // initialize widgets
    mainMenuWidget = new MainMenuWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(mainMenuWidget);

    mainMenuWidget->setVisibility(true);

    pauseMenuWidget = new PauseMenuWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(pauseMenuWidget);
    pauseMenuWidget->setVisibility(false);

    scoreWidget = new BreakoutScoreWidget(&currentLives, &level, &score, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(scoreWidget);

    gameOverWidget = new GameOverWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(gameOverWidget);
    gameOverWidget->setVisibility(false);

    wonWidget = new WonWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(wonWidget);
    wonWidget->setVisibility(false);
}

void BreakoutGameMode::start() {
    scoreWidget->setVisibility(true);
    mainMenuWidget->setVisibility(false);
    bInMainMenu = false;

    loadNextLevel();
    engine->setCursorVisible(false);

    bPaused = false;
    if (board) board->setPaused(true);
}

void BreakoutGameMode::end() {
    // show game over or won widget depending on whether the lives count
    engine->setCursorVisible(true);
    scoreWidget->setVisibility(false);
    if (currentLives <= 0) gameOverWidget->setVisibility(true);
    else {
        // if we have lives left, score is multiplied by lives count
        score *= currentLives;
        wonWidget->setVisibility(true);
    }
    bPaused = true;
    bEndScreen = true;
}

void BreakoutGameMode::reset() {
    currentLives = initialLives - 1;
    score = 0;
    level = 0;
}


void BreakoutGameMode::updateScore(int points) { score += points; }

void BreakoutGameMode::lostBall() {
    currentLives--;

    // if we have no lives left, end the game
    if (currentLives <= 0) {
        currentLives = 0;

        end();
    }
}

void BreakoutGameMode::pause() {
    // pause the game mode and board (so the player can't move)
    GameMode::pause();
    if (pauseMenuWidget)
        pauseMenuWidget->setVisibility(true);
    if (board) board->setPaused(true);
    engine->setCursorVisible(true);
}

void BreakoutGameMode::resume() {
    GameMode::resume();
    if (pauseMenuWidget)
        pauseMenuWidget->setVisibility(false);
    if (board) board->setPaused(false);
    engine->setCursorVisible(false);
}

void BreakoutGameMode::openMainMenu() {
    engine->setCursorVisible(true);

    if (pauseMenuWidget)
        pauseMenuWidget->setVisibility(false);
    if (scoreWidget)
        scoreWidget->setVisibility(false);
    if (board) {
        engine->getScene()->removeObject(board);
        board->destroy();
        board = nullptr;
    }


    if (mainMenuWidget) mainMenuWidget->setVisibility(true);

    if (gameOverWidget)
        gameOverWidget->setVisibility(false);
    if (wonWidget)
        wonWidget->setVisibility(false);

    reset();

    bInMainMenu = true;
    bEndScreen = false;
}


void BreakoutGameMode::loadNextLevel() {
    level++;
    currentLives++;


    auto oldBoard = board;

    std::string levelFile = "levels/level" + std::to_string(level) + ".xml";
    std::filesystem::path xmlPath = (engine->getRuntimePath() / "resources" / levelFile);
    if (!exists(xmlPath)) end();
    
    // create new board and load level
    board = new BreakoutBoard(engine, BreakoutLevelInfo(xmlPath));
    engine->getScene()->addObject(board);
    engine->possess(board);

    engine->getScene()->getActiveCamera()->setLocation(board->getGlobalLocation() + board->getIdealCameraPosition());

    // destroy old board and remove from engine
    if (oldBoard) {
        engine->getScene()->removeObject(oldBoard);
        oldBoard->destroy();
    }

}

void BreakoutGameMode::onKey(int key, int scancode, int action, int mods) {
    if (bInMainMenu) return;

    // toggle pause with escape key
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        if (!bInMainMenu && !bEndScreen)
            bPaused ? resume() : pause();
    }
}
