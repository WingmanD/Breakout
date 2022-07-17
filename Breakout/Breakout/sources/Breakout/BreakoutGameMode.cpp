#include "Breakout/BreakoutGameMode.hpp"

#include <iostream>
#include <future>

#include "Breakout/BreakoutScoreWidget.hpp"
#include "Engine.hpp"
#include "Breakout/BreakoutBoard.hpp"
#include "Breakout/MainMenuWidget.hpp"
#include "Breakout/PauseMenuWidget.hpp"

BreakoutGameMode::BreakoutGameMode(Engine* owner, int lives) : GameMode(owner), initialLives(lives) {
    currentLives = lives - 1;

    mainMenuWidget = new MainMenuWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(mainMenuWidget);

    mainMenuWidget->setVisibility(true);

    pauseMenuWidget = new PauseMenuWidget(this, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(pauseMenuWidget);
    pauseMenuWidget->setVisibility(false);

    scoreWidget = new BreakoutScoreWidget(&currentLives, &level, &score, engine->getWidthRef(), engine->getHeightRef());
    engine->getWidgetManager()->addWidget(scoreWidget);


}

void BreakoutGameMode::start() {
    scoreWidget->setVisibility(true);
    mainMenuWidget->setVisibility(false);
    bInMainMenu = false;

    loadNextLevel();
    engine->setCursorVisible(false);
}

void BreakoutGameMode::end() {
    // todo display score and game over and level, play sound
    std::cout << "Game over" << std::endl;
}

void BreakoutGameMode::reset() {
    currentLives = initialLives;
    score = 0;
    level = 1;
}


void BreakoutGameMode::updateScore(int points) { score += points; }

void BreakoutGameMode::lostBall() {
    currentLives--;

    if (currentLives <= 0) {
        currentLives = 0;

        end();
    }
}

void BreakoutGameMode::pause() {
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


void BreakoutGameMode::loadNextLevel() {
    level++;
    currentLives++;


    auto oldBoard = board;

    std::string levelFile = "levels/level" + std::to_string(3) + ".xml";
    std::filesystem::path xmlPath = (engine->getRuntimePath() / "resources" / levelFile);
    if (!exists(xmlPath)) end();

    std::cout << "loading level " << xmlPath << std::endl;

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

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        if (!bInMainMenu)
            bPaused ? resume() : pause();
    }
}
