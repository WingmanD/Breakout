#include "Breakout/BreakoutBoard.hpp"

#include "BoxCollisionComponent.hpp"
#include "Engine.hpp"
#include "Breakout/BreakoutGameMode.hpp"

#define PHYSICS_DELTA_TIME (1.0f / 120.0f)
#define MAX_BALLS 5

BreakoutBoard::BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level): Player(owningEngine) {
    rowCount = level.rowCount;
    columnCount = level.columnCount;

    // import brick static mesh
    auto meshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/brick/brick.obj",
                                              new Material(
                                                  new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (meshImport.empty()) throw std::runtime_error("BreakoutBoard: brick mesh not found");
    StaticMesh* brickMesh = meshImport[0];

    // calculate brick width and height from brick mesh bounding box
    auto brickAABB = brickMesh->getBoundingBox();
    float brickWidth = std::max(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);
    float brickHeight = std::min(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);

    // calculate cell width and height
    cellWidth = brickWidth + static_cast<float>(level.columnSpacing) * brickWidth * 0.1f;
    cellHeight = brickHeight + static_cast<float>(level.rowSpacing) * brickHeight * 0.1f;

    boardWidth = static_cast<float>(level.columnCount) * cellWidth;
    float boardHeight = static_cast<float>(level.rowCount) * cellHeight;

    // calculate board height (distance between top row of bricks and player) so that the whole board has aspect ratio of 16:9
    totalBoardHeight = 9 / 16.0f * boardWidth;

    // setup background
    auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                               new Material(
                                                   new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("BreakoutBoard: plane not found");
    StaticMesh* planeMesh = planeImport[0];
    planeMesh->getMaterial()->
               setTextureMap(DIFFUSE, engine->getRuntimePath() / "resources" / level.backgroundImagePath);
    background = new StaticMeshComponent(planeMesh);

    boardSize = std::max(boardWidth, totalBoardHeight) * 1.5f;

    planeMesh->getMaterial()->setTextureScale(glm::vec2(boardSize));
    background->attachTo(this);
    background->setScale(glm::vec3(boardSize));
    background->setLocation({0, -0.5, 0});

    // setup walls
    wallTop = new BoxCollisionComponent({boardWidth * 1.2f, 0.2f, cellHeight});
    wallTop->attachTo(this);
    wallTop->setLocation({0, 0, getCellCenter(-1, 0).y});

    wallLeft = new BoxCollisionComponent({cellWidth, 0.2f, totalBoardHeight * 1.2f});
    wallLeft->attachTo(this);
    wallLeft->setLocation({getCellCenter(0, -1).x, 0, 0});

    wallRight = new BoxCollisionComponent({cellWidth, 0.2f, totalBoardHeight * 1.2f});
    wallRight->attachTo(this);
    wallRight->setLocation({getCellCenter(0, level.columnCount).x, 0, 0});

    // setup kill volume under the player
    killVolume = new BoxCollisionComponent({boardWidth * 1.2f, 0.2f, cellHeight});
    killVolume->attachTo(this);
    killVolume->setLocation({
        0.0f,
        0.0f,
        totalBoardHeight / 2.0f + 2 * cellHeight
    });

    // setup player
    paddle = new Paddle(engine, engine->getRuntimePath() / "resources/meshes/player/player.obj",
                        engine->getRuntimePath() / "resources/meshes/player/MetalPipeWallRusty_albedo.png");
    paddle->attachTo(this);
    paddle->setLocation({0, 0, totalBoardHeight / 2});


    // for each brick type in level, create a new mesh by copying the brick mesh, load sound effects
    struct BrickData {
        StaticMesh* mesh = nullptr;
        std::shared_ptr<SoundCue> hitSound;
        std::shared_ptr<SoundCue> breakSound;
    };
    
    std::map<std::string, BrickData> brickDataCache;
    for (auto [name, type] : level.brickTypeMap) {
        auto mesh = new StaticMesh(*brickMesh);
        mesh->getMaterial()->setTextureMap(DIFFUSE, engine->getRuntimePath() / "resources" / type->texturePath);

        std::filesystem::path hitSoundPath = engine->getRuntimePath() / "resources" / type->hitSoundPath;
        std::filesystem::path breakSoundPath = engine->getRuntimePath() / "resources" / type->breakSoundPath;

        std::shared_ptr<SoundCue> hitSound = nullptr;
        std::shared_ptr<SoundCue> breakSound = nullptr;

        if (!type->hitSoundPath.empty())
            hitSound = engine->getSoundEngine()->loadSoundCue(hitSoundPath);
        if (!type->breakSoundPath.empty())
            breakSound = engine->getSoundEngine()->loadSoundCue(breakSoundPath);

        brickDataCache[name] = {
            mesh, hitSound, breakSound
        };
    }

    // create bricks - static mesh is instanced, sounds are shared using pointers
    bricks.resize(level.rowCount, std::vector<BreakoutBrick*>(level.columnCount));
    for (auto i = 0; i < level.rowCount; i++) {
        for (auto j = 0; j < level.columnCount; j++) {
            BreakoutLevelInfo::BrickTypeInfo* brickInfo = level.bricks[i][j];
            if (brickInfo) {
                auto brick = new BreakoutBrick(engine, brickInfo->hitPoints, brickInfo->breakScore,
                                               brickDataCache[brickInfo->name].mesh);
                brick->hitSound = brickDataCache[brickInfo->name].hitSound;
                brick->breakSound = brickDataCache[brickInfo->name].breakSound;

                brick->attachTo(this);

                glm::vec2 cellCenter = getCellCenter(i, j);
                brick->setLocation({
                    cellCenter.x,
                    0,
                    cellCenter.y
                });
                bricks[i][j] = brick;

                if (!(brickInfo->hitPoints == std::numeric_limits<int>::max())) bricksLeft++;
            }
        }
    }

    // load ball static mesh
    auto ballMeshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/sphere/sphere.obj",
                                                  new Material(
                                                      new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (ballMeshImport.empty()) throw std::runtime_error("BreakoutBoard: ball mesh not found");
    ballMesh = ballMeshImport[0];
    ballMesh->getMaterial()->setTextureMap(
        DIFFUSE, engine->getRuntimePath() / "resources/meshes/sphere/2k_moon.jpg");


    // spawn a ball on top of player
    spawnBallAttached();


    // setup ideal camera location
    float idealCameraDistance = std::max(
        (totalBoardHeight * 1.3f) / std::tan(glm::radians(engine->getScene()->getActiveCamera()->getFov())),
        (boardWidth * 1.3f) / (static_cast<float>(engine->getWidth()) / static_cast<float>(
            engine->getHeight()) * std::tan(
            glm::radians(engine->getScene()->getActiveCamera()->getFov()))));

    idealCameraPosition = {0, idealCameraDistance, 0};

    // add light
    light = new Light({0, 0, 0}, {1, 1, 1}, 0.0f);
    engine->getScene()->addLight(light);
    light->attachTo(this);
    light->setLocation({0, 1.0f, 0});


    // initialize powerups
    lightsOutPowerup = new Powerup(this, 5.0, [](BreakoutBoard* board) {
                                       board->engine->getRenderer()->setUnlit(false);
                                   },
                                   [](BreakoutBoard* board) { board->engine->getRenderer()->setUnlit(true); });
    speedUpPowerup = new Powerup(this, 5.0, [](BreakoutBoard* board) {
                                     board->ballSpeed *= 1.5f;
                                     std::cout << "speed up" << std::endl;
                                 },
                                 [](BreakoutBoard* board) {
                                     board->ballSpeed /= 1.5f;
                                     std::cout << "speed up end" << std::endl;
                                 });
    slowMotionPowerup = new Powerup(this, 5.0, [](BreakoutBoard* board) { board->ballSpeed /= 1.5f; },
                                    [](BreakoutBoard* board) { board->ballSpeed *= 1.5f; });
    largeBallPowerup = new Powerup(this, 5.0, [](BreakoutBoard* board) {
                                       for (auto ball : board->balls) ball->scale(glm::vec3(1.5f));
                                   },
                                   [](BreakoutBoard* board) {
                                       for (auto ball : board->balls) ball->scale(glm::vec3(1 / 1.5f));
                                   });
    smallBallPowerup = new Powerup(this, 5.0, [](BreakoutBoard* board) {
                                       for (auto ball : board->balls) ball->scale(glm::vec3(1 / 1.5f));
                                   },
                                   [](BreakoutBoard* board) {
                                       for (auto ball : board->balls) ball->scale(glm::vec3(1.5f));
                                   });

    powerups.emplace_back(lightsOutPowerup);
    powerups.emplace_back(speedUpPowerup);
    powerups.emplace_back(slowMotionPowerup);
    powerups.emplace_back(largeBallPowerup);
    powerups.emplace_back(smallBallPowerup);
}

void BreakoutBoard::tick(double deltaTime) {
    if (bPaused) return;

    // physics substepping - update physics every PHYSICS_DELTA_TIME seconds
    const int fullIterations = static_cast<int>(static_cast<float>(deltaTime) / PHYSICS_DELTA_TIME);
    const float lastIterLength = static_cast<float>(deltaTime) - static_cast<float>(fullIterations) *
        PHYSICS_DELTA_TIME;

    for (int i = 0; i < fullIterations; i++)
        applyBallMovement(PHYSICS_DELTA_TIME);
    applyBallMovement(lastIterLength);

    // add balls that need to be added - this is required because we are iterating over that vector and can't add/remove
    if (!ballsToAdd.empty()) {
        for (auto newBall : ballsToAdd) balls.emplace_back(newBall);
        ballsToAdd.clear();
    }

    // check powerups if they should be deactivated
    for (auto powerup : powerups) if (!powerup->checkActive()) powerup->stop();
}

void BreakoutBoard::onKey(int key, int scancode, int action, int mods) {
    // space key starts the game
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        if (bPaused) { start(); }
        else { std::cout << "cant start - board is not paused"; }
    }
    
}

void BreakoutBoard::onMouseMove(double xpos, double ypos) {
    // set player location to mouse location relative to the whole window
    float newX = Util::clamp(0.05f, 0.95f, static_cast<float>(xpos) / static_cast<float>(engine->getWidth())) *
        boardWidth
        - boardWidth / 2.0f;

    paddle->setLocation({newX, 0, paddle->getLocation().z});
}

void BreakoutBoard::onWindowSizeChange(int width, int height) {
    // window size changed, resize background to fit the screen
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    boardSize = std::max(aspectRatio * 9.0f / 16.0f * boardWidth,
                         1 / aspectRatio * static_cast<float>(rowCount) * cellHeight) * 1.5f;
    background->setScale(glm::vec3(boardSize));

    background->getMesh()->getMaterial()->setTextureScale(glm::vec2(boardSize));
}

void BreakoutBoard::destroy() {
    Player::destroy();

    delete[] powerups.data();

    engine->getScene()->removeLight(light);
    light->destroy();
}

void BreakoutBoard::spawnBallAttached() {
    // spawn a ball on top of player
    auto ball = new BreakoutBall(engine, ballMesh);
    ball->attachTo(paddle);
    ball->setLocation({0, 0, -paddle->getHalfHeight() - ball->getRadius() * 1.1f});

    balls.emplace_back(ball);
}

void BreakoutBoard::start() {
    // launch the ball in random direction
    if (engine->getGameMode()->isPaused()) {
        std::cout << "cant start - gamemode is paused" << std::endl;
        return;
    }

    bPaused = false;
    for (auto ball : balls) {
        ball->attachTo(this);

        // set ball direction to upwards from player but randomize a little bit
        ball->setVelocity(normalize(glm::vec3(Util::random(-3.0f, 3.0f), 0.0f, -1.0f)));
    }
}

void BreakoutBoard::cellIndicesFromBallLocation(BreakoutBall* ball, int& row, int& column) const {
    // get cell indices from ball location, regardless of whether the indices are outside of level grid or not
    const glm::vec3 ballRelativeLocation = glm::vec3(-boardWidth / 2.0f, 0.0f, - totalBoardHeight / 2.0f) -
        ball->getLocation();
    row = - static_cast<int>(ballRelativeLocation.z / cellHeight);
    column = -static_cast<int>(ballRelativeLocation.x / cellWidth);
}

void BreakoutBoard::checkCollision(BreakoutBall* ball) {
    int row, column;
    cellIndicesFromBallLocation(ball, row, column);

    // check collision with surrounding bricks only
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) { checkCollision(ball, i, j); }
    }
}

void BreakoutBoard::checkCollision(BreakoutBall* ball, int row, int column) {
    // check collisions with walls
    ball->collideWith(wallTop);
    ball->collideWith(wallLeft);
    ball->collideWith(wallRight);

    // check collision with player
    ball->collideWith(paddle->getBoxCollision());

    // check if ball is under the player
    if (ball->collideWith(killVolume) || distance(ball->getLocation(), paddle->getLocation()) > totalBoardHeight *
        1.5f) {
        std::erase(balls, ball);
        ball->destroy();

        if (balls.empty()) {
            if (const auto gameMode = dynamic_cast<BreakoutGameMode*>(engine->getGameMode())) {
                gameMode->lostBall();

                bPaused = true;

                if (gameMode->getLivesCount() > 0) spawnBallAttached();
            }
        }
    }

    if (row < 0 || row > rowCount - 1 || column < 0 || column > columnCount - 1) return;


    // check collision with bricks
    auto brick = bricks[row][column];

    // no brick in this cell - nothing to check
    if (!brick) return;

    // check collision with brick
    if (ball->collideWith(brick->getBoxCollision())) {
        brick->hit();
        if (brick->isDestroyed()) {
            bricks[row][column] = nullptr;
            brick->destroy();
            bricksLeft--;
            if (bricksLeft <= 0)
                if (const auto gameMode = dynamic_cast<BreakoutGameMode*>(engine->getGameMode()))
                    gameMode->loadNextLevel();
        }

        // add an additional ball at the current ball location
        if (Util::rollDice(0.1f)) {
            if (balls.size() + ballsToAdd.size() < MAX_BALLS) {
                auto newBall = new BreakoutBall(engine, ballMesh);
                newBall->attachTo(this);

                newBall->setLocation(ball->getLocation());

                newBall->setVelocity(normalize(glm::vec3(Util::random(-1.0f, 1.0f), 0.0f, Util::random(-1.0f, 1.0f))));

                ballsToAdd.emplace_back(newBall);
            }
        }

        // add powerups on brick hit
        addPowerups();
    }
}

glm::vec2 BreakoutBoard::getCellCenter(int row, int column) const {
    // get cell center location, regardless of whether the indices are outside of level grid or not
    return {
        static_cast<float>(column) * cellWidth + cellWidth / 2.0f - boardWidth / 2.0f,
        static_cast<float>(row + 1) * cellHeight + cellHeight / 2.0f - totalBoardHeight / 2.0f
    };
}

void BreakoutBoard::applyBallMovement(float deltaTime) {
    // check collisions with all balls and move them accordingly
    for (auto ball : balls) {
        checkCollision(ball);
        ball->move(ball->getVelocity() * ballSpeed * deltaTime);
    }
}

void BreakoutBoard::addPowerups() {
    
    if (Util::rollDice(1.0f)) {
        if (!speedUpPowerup->checkActive() && !slowMotionPowerup->checkActive()) {
            if (Util::rollDice(0.5f))
                speedUpPowerup->start();
            else
                slowMotionPowerup->start();
        }
    }

    /*
    if (Util::rollDice(0.1f)) {
        if (!largeBallPowerup->checkActive() && !smallBallPowerup->checkActive()) {
            if (Util::rollDice(0.5f))
                largeBallPowerup->start();
            else
                smallBallPowerup->start();
        }
    }

    if (Util::rollDice(0.1f))
        if (!lightsOutPowerup->checkActive()) lightsOutPowerup->start();
        */


}
