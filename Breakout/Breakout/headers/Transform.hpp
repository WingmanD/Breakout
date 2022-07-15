#pragma once

#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class Transform {

protected:
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 Location = glm::vec3(0), Rotation = glm::vec3(0), Scale = glm::vec3(1);
public:
    [[nodiscard]] Transform* getParent() const { return parent; }
    
    [[nodiscard]] glm::vec3 getLocation() const { return Location; }
    [[nodiscard]] glm::vec3 getRotation() const { return Rotation; }
    [[nodiscard]] glm::vec3 getScale() const { return Scale; }

    void setLocation(glm::vec3 newLocation) { Location = newLocation; }
    void setRotation(glm::vec3 newRotation) { Rotation = newRotation; }
    void setScale(glm::vec3 newScale) { Scale = newScale; }

    void move(glm::vec3 direction) { Location += direction; }
    void rotate(glm::vec3 eulerAngles) { Rotation += eulerAngles; }
    void scale(glm::vec3 factor) { Scale *= factor; }

    [[nodiscard]] glm::mat4 getModelMatrix() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    [[nodiscard]] glm::vec3 getGlobalLocation() const;
    [[nodiscard]] glm::vec3 getGlobalRotation() const;

    [[nodiscard]] glm::vec3 toGlobal(glm::vec3 relativeLocation) const;
    [[nodiscard]] glm::vec3 toLocal(glm::vec3 globalLocation) const;
    
    virtual void attachTo(Transform* newParent);
    virtual void detach();

    virtual void destroy();
};
