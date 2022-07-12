#pragma once

#include "Transform.hpp"

class Renderer;

class Camera : public Transform {
    glm::vec3 center = glm::vec3(0, 0, 0);
    
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    
    glm::vec3 viewUp = glm::vec3(0, 1, 0);
    glm::vec3 front = glm::vec3(1, 0, 0);
    glm::vec3 right = glm::vec3(0, 0, 1);

    float fov = 45.0f;
    float nearPlane = 0.01f;
    float farPlane = 100.0f;
    float focalDistance = 5.0f;

    int* width;
    int* height;

public:
    Camera(int* width, int* height, glm::vec3 eyeLocation, glm::vec3 center);

    void rotate(glm::vec3 rot);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix() const;

    [[nodiscard]] glm::vec3 getViewUp() const;
    [[nodiscard]] glm::vec3 getFront() const;
    [[nodiscard]] glm::vec3 getRight() const;

    [[nodiscard]] float getFov() const { return fov; }
    [[nodiscard]] float getNearPlane() const { return nearPlane; }
    [[nodiscard]] float getFarPlane() const { return farPlane; }
    [[nodiscard]] int getWidth() const { return *width; }
    [[nodiscard]] int getHeight() const { return *height; }

    void setFov(float newFov) { this->fov = newFov; }
    void setNearPlane(float newNearPlane) { nearPlane = newNearPlane; }
    void setNarPlane(float newFarPlane) { farPlane = newFarPlane; }
    void setWidth(int* newWidth) { this->width = newWidth; }
    void setHeight(int* newHeight) { this->height = newHeight; }
    
};
