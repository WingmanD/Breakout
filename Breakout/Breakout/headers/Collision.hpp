#pragma once
#include <glm/vec3.hpp>

struct CollisionResult {
    bool bCollided = false;
    glm::vec3 collisionPoint = {0, 0, 0};
    glm::vec3 collisionNormal = {1, 0, 0};
};

class Collision {
public:
    virtual CollisionResult collide(Collision* other) = 0;
    virtual bool isPointInside(const glm::vec3& point) = 0;

    virtual ~Collision() = default;
};
