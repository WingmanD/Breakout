#pragma once

#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

/**
  Transform class handles the location, rotation and scale of objects.
  It implements a scene graph where objects can be attached to each other.
 */

class Transform {

protected:
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::vec3 Location = glm::vec3(0), Rotation = glm::vec3(0), Scale = glm::vec3(1);
public:
    /**
      Get parent from scene graph.
     */
    [[nodiscard]] Transform* getParent() const { return parent; }

    /**
      Get relative location of object to its parent. Returns global location if no parent.
     */
    [[nodiscard]] glm::vec3 getLocation() const { return Location; }

    /**
      Get relative rotation of object to its parent. Returns global rotation if no parent.
     */
    [[nodiscard]] glm::vec3 getRotation() const { return Rotation; }

    /**
     Get relative rotation of object to its parent. Returns global rotation if no parent.
     */
    [[nodiscard]] glm::vec3 getScale() const { return Scale; }

    /**
      Set relative location of object to its parent. Sets global location if no parent.
     */
    void setLocation(glm::vec3 newLocation) { Location = newLocation; }

    /**
     * Set relative rotation of object to its parent. Sets global rotation if no parent.
     */
    void setRotation(glm::vec3 newRotation) { Rotation = newRotation; }

    /**
     * Set relative scale of object to its parent. Sets global scale if no parent.
     */
    void setScale(glm::vec3 newScale) { Scale = newScale; }

    /**
       Move object by a given amount.
    */
    void move(glm::vec3 direction) { Location += direction; }

    /**
      Rotate object by a given amount.
     */
    void rotate(glm::vec3 eulerAngles) { Rotation += eulerAngles; }

    /**
      Scale object by a given amount.
     */
    void scale(glm::vec3 factor) { Scale *= factor; }

    /**
      Get model matrix of this object's coordinate system
     */
    [[nodiscard]] glm::mat4 getModelMatrix() const;

    /**
      Get view matrix of this object's coordinate system
      NOT IMPLEMENTED
     */
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    /**
     Get location in global coordinate system.
     */
    [[nodiscard]] glm::vec3 getGlobalLocation() const;

    /**
     Get rotation in global coordinate system.
     */
    [[nodiscard]] glm::vec3 getGlobalRotation() const;

    /**
     Get scale in global coordinate system.
     */
    [[nodiscard]] glm::vec3 getGlobalScale() const;

    /**
      Get global location of a point in local coodinate system of this object.
     */
    [[nodiscard]] glm::vec3 toGlobal(glm::vec3 relativeLocation) const;

    /**
      Get relative location of a point in global coodinate system.
     */
    [[nodiscard]] glm::vec3 toLocal(glm::vec3 globalLocation) const;

    /**
      Get child objects attached to this object.
     */
    [[nodiscard]] std::vector<Transform*> getChildren() const { return children; }

    /**
      Attach a child object to this object. The child object will be deleted when this object is deleted.
      Preserves relative location and scale to this object, relative rotation is set to 0.
     */
    virtual void attachTo(Transform* newParent);

    /**
      Detach a child object from this object.
     */
    virtual void detach();

    /**
      Destroy this object and all children.
     */
    virtual void destroy();

    virtual ~Transform();
};
