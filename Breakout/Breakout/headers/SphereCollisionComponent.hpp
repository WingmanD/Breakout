#pragma once
#include "Collision.hpp"
#include "SceneComponent.hpp"

/**
 *  Sphere collision component that can be used to detect collisions with other collision components.
 * 
 */
class SphereCollisionComponent : public SceneComponent, public Collision {
    float radius = 0;
public:
    explicit SphereCollisionComponent(const float radius)
        : radius(radius) {}

    /**
     * Check if this sphere collides with another collider.
     * 
     */
    CollisionResult collide(Collision* other) override;

    /**
     *  Check if point with global coordinates is inside this sphere.
     * 
     */
    bool isPointInside(const glm::vec3& globalPoint) override;

    
    [[nodiscard]] float getRadius() const { return getGlobalScale().x * radius; }
    void setRadius(const float newRadius) { this->radius = newRadius * getGlobalScale().x; }

    void init() override {}
};
