#include "Breakout/BreakoutBoard.hpp"

#include "BoxCollisionComponent.hpp"
#include "Engine.hpp"

#define PHYSICS_DELTA_TIME (1.0f / 240.0f)

BreakoutBoard::BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level): Player(owningEngine) {
    rowCount = level.rowCount;
    columnCount = level.columnCount;

    auto meshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/brick/brick.obj",
                                              new Material(
                                                  new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (meshImport.empty()) throw std::runtime_error("BreakoutBoard: brick mesh not found");
    StaticMesh* brickMesh = meshImport[0];

    for (auto [name, type] : level.brickTypeMap) {
        brickNameSMeshMap[name] = new StaticMesh(*brickMesh);
        brickNameSMeshMap[name]->setMaterial(new Material(new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
        brickNameSMeshMap[name]->getMaterial()->setTextureMap(
            DIFFUSE, engine->getRuntimePath() / "resources" / type->texturePath);
    }

    auto brickAABB = brickMesh->getBoundingBox();
    brickWidth = std::max(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);
    brickHeight = std::min(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);

    auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                               new Material(
                                                   new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("BreakoutBoard: plane not found");

    StaticMesh* planeMesh = planeImport[0];
    planeMesh->getMaterial()->
               setTextureMap(DIFFUSE, engine->getRuntimePath() / "resources" / level.backgroundImagePath);

    auto background = new StaticMeshComponent(planeMesh);

    cellWidth = brickWidth + level.columnSpacing * brickWidth * 0.1f;
    cellHeight = brickHeight + level.rowSpacing * brickHeight * 0.1f;

    boardWidth = static_cast<float>(level.columnCount) * cellWidth;
    float boardHeight = static_cast<float>(level.rowCount) * cellHeight;

    totalBoardHeight = 9 / 16.0f * boardWidth;

    float backgroundScale = std::max(boardWidth * 1.1f, boardHeight * 1.1f);

    //std::cout << "tex scale " << planeMesh->getMaterial()->getTextureScale().x << std::endl;
    //todo fix this
    //planeMesh->getMaterial()->setTextureScale(glm::vec2(backgroundScale));
    background->setScale(glm::vec3(backgroundScale));
    background->attachTo(this);


    bricks.resize(level.rowCount, std::vector<Brick*>(level.columnCount));
    for (auto i = 0; i < level.rowCount; i++) {
        for (auto j = 0; j < level.columnCount; j++) {
            if (level.bricks[i][j]) {
                auto brick = new Brick(level.bricks[i][j]);
                brick->meshComp = new StaticMeshComponent(brickNameSMeshMap[brick->info->name]);

                brick->boxCollision = new BoxCollisionComponent(brick->meshComp->getMesh()->getBoundingBox());
                brick->boxCollision->attachTo(brick->meshComp);

                brick->meshComp->attachTo(this);

                brick->meshComp->setLocation({
                    static_cast<float>(j) * cellWidth + cellWidth / 2.0f - boardWidth / 2.0f,
                    0.1f,
                    static_cast<float>(i) * (cellHeight) + cellHeight / 2.0f
                    - totalBoardHeight / 2.0f
                });
                /*std::cout << engine->getRuntimePath() / "resources" / level.bricks[i][j]->hitSoundPath << std::endl;
                std::cout << level.bricks[i][j]->hitSoundPath << std::endl;*/

                brick->hitSound = engine->getSoundEngine()->loadSoundCue(
                    engine->getRuntimePath() / "resources" / level.bricks[i][j]->hitSoundPath);
                brick->breakSound = engine->getSoundEngine()->loadSoundCue(
                    engine->getRuntimePath() / "resources" / level.bricks[i][j]->breakSoundPath);

                bricks[i][j] = brick;
            }
        }
    }

    // todo move player to separate class
    auto playerMeshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/player/player.obj",
                                                    new Material(
                                                        new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (playerMeshImport.empty()) throw std::runtime_error("BreakoutBoard: player mesh not found");
    StaticMesh* playerMesh = playerMeshImport[0];

    playerMesh->getMaterial()->setTextureMap(
        DIFFUSE, engine->getRuntimePath() / "resources/meshes/player/MetalPipeWallRusty_albedo.png");

    auto playerAABB = playerMesh->getBoundingBox();
    playerWidth = playerAABB.max.x - playerAABB.min.x;
    //todo watch out for z coord
    playerHeight = playerAABB.max.z - playerAABB.min.z;

    player = new StaticMeshComponent(playerMesh);
    auto playerBoxCollision = new BoxCollisionComponent(playerMesh->getBoundingBox());
    playerBoxCollision->attachTo(player);

    player->attachTo(this);

    player->setLocation({
        0.0f,
        0.1f,
        totalBoardHeight / 2.0f
    });

    auto ballMeshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/sphere/sphere.obj",
                                                  new Material(
                                                      new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (ballMeshImport.empty()) throw std::runtime_error("BreakoutBoard: ball mesh not found");

    ballMesh = ballMeshImport[0];
    ballMesh->getMaterial()->setTextureMap(
        DIFFUSE, engine->getRuntimePath() / "resources/meshes/sphere/2k_moon.jpg");

    spawnBall();

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

    const int fullIterations = static_cast<int>(static_cast<float>(deltaTime) / (1.0f / 120.0f));
    const float lastIterLength = static_cast<float>(deltaTime) - static_cast<float>(fullIterations) * (1.0f / 120.0f);

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


    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        if (bDisabled)
            start();

}

void BreakoutBoard::onMouseMove(double xpos, double ypos) {
    float newX = Util::clamp(0.05f, 0.95f, static_cast<float>(xpos) / static_cast<float>(engine->getWidth())) *
        boardWidth
        - boardWidth / 2.0f;
    player->setLocation({newX, player->getLocation().y, player->getLocation().z});
}

void BreakoutBoard::spawnBall() {
    auto ball = new Ball(new StaticMeshComponent(new StaticMesh(*ballMesh)));

    ball->meshComp->attachTo(player);

    ball->meshComp->setLocation({
        0.0f,
        0.0f,
        -(playerHeight / 2.0f + ball->radius * 2.0f)
    });

    std::cout << "ball radius " << ball->radius << std::endl;

    balls.emplace_back(ball);

}

void BreakoutBoard::start() {
    bDisabled = false;
    for (auto ball : balls) {
        ball->meshComp->attachTo(this);

        //ball->velocity = normalize(glm::vec3(0.0f, 0.0f, -1.0f));
        ball->velocity = normalize(glm::vec3(Util::random(-3.0f, 3.0f), 0.0f, -1.0f));
    }
}

void BreakoutBoard::cellIndicesFromBallLocation(Ball* ball, int& row, int& column) {
    glm::vec3 ballRelativeLocation = glm::vec3(-boardWidth / 2.0f, 0.0f, - totalBoardHeight / 2.0f) -
        ball->meshComp->getLocation();
    row = - static_cast<int>(ballRelativeLocation.z / cellHeight);
    column = -static_cast<int>(ballRelativeLocation.x / cellWidth);
}

void BreakoutBoard::checkCollision(Ball* ball) {
    int row, column;
    cellIndicesFromBallLocation(ball, row, column);
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) { checkCollision(ball, i, j); }
    }
}

void BreakoutBoard::checkCollision(Ball* ball, int row, int column) {
    //todo update this to use BoxCollision for walls and player

    // todo extract ball and player locations here
    // check collision with top wall
    if (row < 0) {
        float topWallZ = - totalBoardHeight / 2.0f;
        float distance = ball->meshComp->getLocation().z - ball->radius - topWallZ;
        if (distance < 0) {
            ball->meshComp->move(
                glm::vec3(0.0f, 0.0f, 1.0f) * -distance);
            ball->velocity = reflect(ball->velocity, glm::vec3(0.0f, 0.0f, 1.0f));
            return;
        }
    }

    // check collision with left wall
    if (column < 0) {
        float leftWallX = -boardWidth / 2.0f;
        float distance = ball->meshComp->getLocation().x - ball->radius - leftWallX;
        if (distance < 0) {
            ball->meshComp->move(
                glm::vec3(1.0f, 0.0f, 0.0f) * -distance);
            ball->velocity = reflect(ball->velocity, glm::vec3(1.0f, 0.0f, 0.0f));
            return;
        }
    }

    // check collision with right wall
    if (column > columnCount - 1) {
        float rightWallX = boardWidth / 2.0f;
        float distance = rightWallX - ball->meshComp->getLocation().x + ball->radius;
        if (distance < 0) {
            ball->meshComp->move(
                glm::vec3(-1.0f, 0.0f, 0.0f) * -distance);
            ball->velocity = reflect(ball->velocity, glm::vec3(-1.0f, 0.0f, 0.0f));
            return;
        }
    }

    // check collision with player
    float distanceFromPlayerUp = player->getLocation().z - playerHeight - ball->meshComp->getLocation().z + ball->
        radius;
    if (distanceFromPlayerUp <= 0 && abs(distanceFromPlayerUp) < ball->radius) {
        float distanceFromPlayerLR = abs(
            ball->meshComp->getLocation().x - ball->radius - player->getLocation().x);

        if (distanceFromPlayerLR < playerWidth / 2.0f) {
            ball->meshComp->move(
                glm::vec3(0.0f, 0.0f, -1.0f) * -distanceFromPlayerUp);
            ball->velocity = reflect(ball->velocity, glm::vec3(0.0f, 0.0f, -1.0f));
            return;
        }
    }

    // check collision with bricks

    if (row < 0 || row > rowCount - 1 || column < 0 || column > columnCount - 1) return;

    // no brick in this cell - nothing to check

    auto brick = bricks[row][column];
    if (!brick) return;

    CollisionResult hit = ball->sphereCollision->collide(brick->boxCollision);
    if (hit.bCollided) {
        ball->meshComp->move(
            hit.collisionNormal * (ball->radius - distance(ball->meshComp->getGlobalLocation(), hit.collisionPoint)));
        ball->velocity = reflect(ball->velocity, hit.collisionNormal);
        brick->hit();
        if (brick->bIsDestroyed) {
            bricks[row][column] = nullptr;
            delete brick;
        }
    }
}

//todo use this in constructor
glm::vec2 BreakoutBoard::getCellCenter(int row, int column) {
    return glm::vec2(
        static_cast<float>(column) * cellWidth + cellWidth / 2.0f - boardWidth / 2.0f,
        static_cast<float>(row) * cellHeight + cellHeight / 2.0f - totalBoardHeight / 2.0f
    );
}

void BreakoutBoard::applyBallMovement(float deltaTime) {
    for (auto ball : balls) {
        checkCollision(ball);
        ball->meshComp->move(ball->velocity * ballSpeed * deltaTime);
    }
}
