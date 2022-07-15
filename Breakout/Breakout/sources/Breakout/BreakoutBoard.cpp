#include "Breakout/BreakoutBoard.hpp"

#include "BoxCollisionComponent.hpp"
#include "Engine.hpp"
#include "Breakout/BreakoutGameMode.hpp"

#define PHYSICS_DELTA_TIME (1.0f / 240.0f)

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
    auto background = new StaticMeshComponent(planeMesh);
    
    float backgroundScale = std::max(boardWidth, boardHeight) * 1.5f;

    planeMesh->getMaterial()->setTextureScale(glm::vec2(backgroundScale));
    background->setScale(glm::vec3(backgroundScale));
    background->attachTo(this);
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
        brickDataCache[name] = {
            mesh, engine->getSoundEngine()->loadSoundCue(engine->getRuntimePath() / "resources" / type->hitSoundPath),
            engine->getSoundEngine()->loadSoundCue(engine->getRuntimePath() / "resources" / type->breakSoundPath)
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
    spawnBall();


    // setup ideal camera location
    auto camera = new Camera(engine->getWidthRef(), engine->getHeightRef(), this->getLocation() + glm::vec3(0, 2, 0),
                             this->getLocation());

    float cameraDistance = std::max((totalBoardHeight * 1.3f) / std::tan(glm::radians(camera->getFov())),
                                    (boardWidth * 1.3f) / (static_cast<float>(engine->getWidth()) / static_cast<float>(
                                        engine->getHeight()) * std::tan(
                                        glm::radians(camera->getFov()))));

    camera->setLocation(this->getLocation() + glm::vec3(0, cameraDistance, 0));

    engine->getScene()->setActiveCamera(camera);

}

void BreakoutBoard::tick(double deltaTime) {
    if (bDisabled) return;

    // physics substepping - update physics every PHYSICS_DELTA_TIME seconds
    const int fullIterations = static_cast<int>(static_cast<float>(deltaTime) / PHYSICS_DELTA_TIME);
    const float lastIterLength = static_cast<float>(deltaTime) - static_cast<float>(fullIterations) *
        PHYSICS_DELTA_TIME;

    for (int i = 0; i < fullIterations; i++)
        applyBallMovement(PHYSICS_DELTA_TIME);
    applyBallMovement(lastIterLength);

}

void BreakoutBoard::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, 1, 0}); }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, -1, 0}); }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({-1, 0, 0}); }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({1, 0, 0}); }
    //else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, 0, 1}); }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        engine->getScene()->getActiveCamera()->move({0, 0, -1});
    else if (key == GLFW_KEY_ENTER == GLFW_PRESS) { }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) { ballSpeed *= 2; }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) { ballSpeed /= 2; }

    // space key starts the game
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        if (bDisabled)
            start();

}

void BreakoutBoard::onMouseMove(double xpos, double ypos) {
    // set player location to mouse location relative to the whole window
    float newX = Util::clamp(0.05f, 0.95f, static_cast<float>(xpos) / static_cast<float>(engine->getWidth())) *
        boardWidth
        - boardWidth / 2.0f;

    paddle->setLocation({newX, 0, paddle->getLocation().z});
}

void BreakoutBoard::spawnBall() {
    // spawn a ball on top of player
    auto ball = new BreakoutBall(engine, ballMesh);
    ball->attachTo(paddle);
    ball->setLocation({0, 0, -paddle->getHalfHeight() - ball->getRadius() * 1.1f});

    balls.emplace_back(ball);
}

void BreakoutBoard::start() {
    // launch the ball in random direction
    bDisabled = false;
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
#

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
    if (ball->collideWith(killVolume)) {
        std::erase(balls, ball);
        ball->destroy();

        if (balls.empty()) {
            if (const auto gameMode = dynamic_cast<BreakoutGameMode*>(engine->getGameMode())) {
                gameMode->lostBall();
                bDisabled = true;
                if (gameMode->getLivesCount() > 0) spawnBall();
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
        }
    }
}

glm::vec2 BreakoutBoard::getCellCenter(int row, int column) const {
    // get cell center location, regardless of whether the indices are outside of level grid or not
    return {
        static_cast<float>(column) * cellWidth + cellWidth / 2.0f - boardWidth / 2.0f,
        static_cast<float>(row) * cellHeight + cellHeight / 2.0f - totalBoardHeight / 2.0f
    };
}

void BreakoutBoard::applyBallMovement(float deltaTime) {
    // check collisions with all balls and move them accordingly
    for (const auto ball : balls) {
        checkCollision(ball);
        ball->move(ball->getVelocity() * ballSpeed * deltaTime);
    }
}
