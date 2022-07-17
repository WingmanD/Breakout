#pragma once
#include "Collision.hpp"
#include "SceneComponent.hpp"
#include "StaticMesh.hpp"

/**
 *  BoxCollisionComponent handles collisions with other colliders using axis aligned bounding box.
 * 
 */
class BoxCollisionComponent : public SceneComponent, public Collision {
    BoundingBox aabb;
public:
    explicit BoxCollisionComponent(const BoundingBox& aabb)
        : aabb(aabb) {}

    explicit BoxCollisionComponent(const glm::vec3& size);

    [[nodiscard]] BoundingBox getBoundingBox() const {
        const glm::vec3 globalLocation = getGlobalLocation();
        return {globalLocation + aabb.min, globalLocation + aabb.max};
    }

    /**
     *  Checks if this collider collides with another collider.
     */
    CollisionResult collide(Collision* other) override;

    /**
     *  Check if point (in global coords) is inside bounding box.
     */
    bool isPointInside(const glm::vec3& globalPoint) override;

    void init() override {}
};
