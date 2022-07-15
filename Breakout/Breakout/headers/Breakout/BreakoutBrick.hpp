#pragma once

#include "Object.hpp"
#include "SoundCue.hpp"
#include "StaticMeshComponent.hpp"
#include "BoxCollisionComponent.hpp"

class BreakoutBrick : public Object {
    int HP = 1;
    int score = 10;

    bool bIsDestroyed = false;

    StaticMeshComponent* meshComp = nullptr;
    BoxCollisionComponent* boxCollision = nullptr;

public:
    std::shared_ptr<SoundCue> hitSound = nullptr;
    std::shared_ptr<SoundCue> breakSound = nullptr;

    BreakoutBrick(Engine* owner, int HP, int score, StaticMesh* mesh);
    void hit();

    [[nodiscard]] bool isDestroyed() const { return bIsDestroyed; }
    [[nodiscard]] BoxCollisionComponent* getBoxCollision() const { return boxCollision; }
};
