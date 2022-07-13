#pragma once

#include <iostream>
#include <random>

#include "Shader.hpp"
#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>

#include "Object.hpp"
#include "StaticMesh.hpp"
#include "GLFW/glfw3.h"


class Util {
public:
    /*static bool isPointInsideObject(Object* obj, glm::vec3 point) {
        std::default_random_engine generator(glfwGetTime());
        const std::uniform_real_distribution<float> distribution(0.0, 1.0);

        glm::vec3 rayDirection = glm::vec3(distribution(generator), distribution(generator), distribution(generator)) -
            point;

        rayDirection = normalize(rayDirection);

        int count = 0;
        for (const auto triangle : obj->mesh->triangles) {
            RayTriangleIntersectionResult result = rayTriangleIntersection(
                point, rayDirection, triangle);
            if (result.intersected) count++;
        }

        return count % 2 != 0;
    }*/


    /*struct RayTriangleIntersectionResult {
        bool intersected = false;
        glm::vec3 intersectionPoint = glm::vec3(0);
    };

    static RayTriangleIntersectionResult rayTriangleIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection,
                                                                 Triangle triangle) {

        RayTriangleIntersectionResult result;

        glm::vec3 v0 = triangle.v0->position;
        glm::vec3 v1 = triangle.v1->position;
        glm::vec3 v2 = triangle.v2->position;

        glm::vec3 e0 = v1 - v0;
        glm::vec3 e1 = v2 - v0;

        // calculate plane
        glm::vec3 n = triangle.getNormal();
        float D = -dot(n, v0);

        // calculate intersection with plane
        float denominator = dot(n, rayDirection);
        if (denominator == 0.0f) return result;

        float t = -(dot(n, rayOrigin) + D) / denominator;
        if (t < 0) return result;
        glm::vec3 intersection = rayOrigin + t * rayDirection;

        // check if intersection is inside triangle
        e0 = triangle.v1->position - triangle.v0->position;
        glm::vec3 v0P = intersection - triangle.v0->position;
        glm::vec3 C = cross(e0, v0P);

        if (dot(n, C) < 0) return result;

        e1 = triangle.v2->position - triangle.v1->position;
        glm::vec3 v1P = intersection - triangle.v1->position;
        C = cross(e1, v1P);
        if (dot(n, C) < 0) return result;

        glm::vec3 e2 = triangle.v0->position - triangle.v2->position;

        glm::vec3 v2P = intersection - triangle.v2->position;
        C = cross(e2, v2P);
        if (dot(n, C) < 0) return result;

        result.intersected = true;
        result.intersectionPoint = intersection;

        return result;
    }*/

    template <typename T>
    static float clamp(T min, T max, T value) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    template <typename T>
    static T random(T min, T max) {
        std::default_random_engine generator(glfwGetTime());
        const std::uniform_real_distribution<float> distribution(min, max);
        
        return distribution(generator);
    }

    static std::vector<std::string> split(const std::string& input, char delimiter) {
        std::vector<std::string> out;
        std::stringstream ss(input);
        std::string tmp;

        while (getline(ss, tmp, delimiter)) out.push_back(tmp);
        return out;
    }

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) return str;
        
        size_t last = str.find_last_not_of(' ');
        
        return str.substr(first, (last - first + 1));
    }

    static float tetrahedronVolume(const Triangle& triangle, const glm::vec3& top) {
        return (1.0f / 6.0f) * glm::abs(glm::dot(triangle.v0->position - top,
                                                 glm::cross(triangle.v1->position - top, triangle.v2->position - top)));
    }
};
