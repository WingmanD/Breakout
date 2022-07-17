#pragma once
#include "Light.hpp"
#include "Object.hpp"

#include "SphereCollisionComponent.hpp"
#include "StaticMeshComponent.hpp"

class BreakoutBall : public Object {
    StaticMeshComponent* meshComp;
    SphereCollisionComponent* sphereCollision;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

    float radius = 0;

    Light* light;

public:
    explicit BreakoutBall(Engine* owner, StaticMesh* mesh);

    [[nodiscard]] SphereCollisionComponent* getSphereCollision() const { return sphereCollision; }

    [[nodiscard]] float getRadius() const { return radius; }
    [[nodiscard]] glm::vec3 getVelocity() const { return velocity; }

    void setRadius(const float newRadius) { this->radius = newRadius; }
    void setVelocity(const glm::vec3& newVelocity) { this->velocity = newVelocity; }

    bool collideWith(Collision* other);

    void destroy() override;
};
