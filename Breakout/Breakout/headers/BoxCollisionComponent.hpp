#pragma once
#include "Collision.hpp"
#include "SceneComponent.hpp"
#include "StaticMesh.hpp"

class BoxCollisionComponent : public SceneComponent, public Collision {
    BoundingBox aabb;
public:
    explicit BoxCollisionComponent(const BoundingBox& aabb)
        : aabb(aabb) {}

    [[nodiscard]] BoundingBox getBoundingBox() const {
        const glm::vec3 globalLocation = getGlobalLocation();
        return { globalLocation + aabb.min, globalLocation + aabb.max };
    }

    CollisionResult collide(Collision* other) override;

    bool isPointInside(const glm::vec3& point) override;
    
    void init() override {} 
};
