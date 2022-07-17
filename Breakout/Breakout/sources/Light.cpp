#include "Light.hpp"

#include <glm/vec3.hpp>

Light::Light(glm::vec3 position, glm::vec3 color, float intensity) {
    setLocation(position);
    this->color = color;
    this->intensity = intensity;
}

glm::vec3 Light::getColor() const { return color; }
float Light::getIntensity() const { return intensity; }

void Light::setColor(glm::vec3 newColor) { this->color = newColor; }
void Light::setIntensity(float newIntensity) { this->intensity = newIntensity; }
