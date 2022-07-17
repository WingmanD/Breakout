#pragma once

#include <random>

#include <glm/geometric.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "StaticMesh.hpp"
#include "GLFW/glfw3.h"


/**
 *  Helper math class.
 */
class Util {
public:
    /**
     *  Clamp value between min and max
     */
    template <typename T>
    static float clamp(T min, T max, T value) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    /**
     *  Returns a random number between min and max.
     */
    template <typename T>
    static T random(T min, T max) {
        std::default_random_engine generator(glfwGetTime());
        const std::uniform_real_distribution<float> distribution(min, max);

        return distribution(generator);
    }

    /**
     *  Random probability with chance.
     */
    static bool rollDice(float chance) { return random(0.0f, 1.0f) <= chance; }

    /**
     *  Split string into tokens using delimiter.
     */
    static std::vector<std::string> split(const std::string& input, char delimiter) {
        std::vector<std::string> out;
        std::stringstream ss(input);
        std::string tmp;

        while (getline(ss, tmp, delimiter)) out.push_back(tmp);
        return out;
    }
    
    /**
     *  Trim string, front and back.
     */
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) return str;

        size_t last = str.find_last_not_of(' ');

        return str.substr(first, (last - first + 1));
    }

    /**
     *  Calculate tetrahedron volume from triangle base and top point
     */
    static float tetrahedronVolume(const Triangle& triangle, const glm::vec3& top) {
        return (1.0f / 6.0f) * glm::abs(dot(triangle.v0->position - top,
                                            cross(triangle.v1->position - top, triangle.v2->position - top)));
    }

    /**
     *  Snap direction vector to nearest axis.
     */
    static glm::vec3 SnapToAxis(const glm::vec3& vec) {
        const float x = abs(vec.x);
        const float y = abs(vec.y);
        const float z = abs(vec.z);

        if (x > y && x > z) return {glm::sign(vec.x), 0, 0};

        if (y > x && y > z) return {0, glm::sign(vec.y), 0};

        return {0, 0, glm::sign(vec.z)};
    }
};
