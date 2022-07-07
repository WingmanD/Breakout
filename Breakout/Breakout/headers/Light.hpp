﻿#pragma once
#include "Transform.hpp"

class Light : public Transform {
    glm::vec3 color{1, 1, 1};
    float intensity = 1.0f;
public:
    Light(glm::vec3 position, glm::vec3 color, float intensity);

    glm::vec3 getColor();
    float getIntensity();

    void setColor(glm::vec3 newColor);
    void setIntensity(float newIntensity);

};
