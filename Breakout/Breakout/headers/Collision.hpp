#pragma once
#include <glm/vec3.hpp>

struct CollisionResult {
    bool bCollided = false;
    glm::vec3 collisionPoint = {0, 0, 0};
    glm::vec3 collisionNormal = {1, 0, 0};
};

/**
 * This class is used to detect collisions between two objects.
 * 
 */
class Collision {
public:
    /**
     * This function is used to detect collisions between two objects.
     */
    virtual CollisionResult collide(Collision* other) = 0;

    /**
     * Check if point (in global coords) is inside collider.
     */
    virtual bool isPointInside(const glm::vec3& globalPoint) = 0;

    virtual ~Collision() = default;
};
