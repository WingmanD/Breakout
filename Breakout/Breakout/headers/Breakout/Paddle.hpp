#pragma once
#include "BoxCollisionComponent.hpp"
#include "Object.hpp"
#include "StaticMeshComponent.hpp"

class Paddle : public Object {
    StaticMeshComponent* meshComp;
    BoxCollisionComponent* boxCollision;

    float halfHeight = 0;
public:
    Paddle(Engine* const engine, const std::filesystem::path& meshPath, const std::filesystem::path& texturePath);


    [[nodiscard]] BoxCollisionComponent* getBoxCollision() const { return boxCollision; }
    [[nodiscard]] float getHalfHeight() const { return halfHeight; }
};
