#pragma once
#include "Collision.hpp"
#include "SceneComponent.hpp"


//todo handle global scale for radius
class SphereCollisionComponent : public SceneComponent, public Collision{
    float radius = 0;
public:
    explicit SphereCollisionComponent(const float radius)
        : radius(radius) {}

    CollisionResult collide(Collision* other) override;
    bool isPointInside(const glm::vec3& point) override;

    [[nodiscard]] float getRadius() const { return radius; }
    void setRadius(const float newRadius) { this->radius = newRadius; }

    void init() override {}
};
