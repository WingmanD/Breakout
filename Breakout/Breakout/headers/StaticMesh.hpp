#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Drawable.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "assimp/mesh.h"

struct Vertex {
    int index = -1;
    glm::vec3 position = {0, 0, 0};
    glm::vec3 colour = {0.5f, 0.5f, 0.5f};
    glm::vec3 normal = {0, 0, 0};
    glm::vec2 uv = {0, 0};

    Vertex() = default;

    Vertex(int index, const float x, const float y, const float z) {
        this->index = index;
        position.x = x;
        position.y = y;
        position.z = z;
    }

    Vertex(int index, glm::vec3 pos) {
        this->index = index;
        position = pos;
    }
};

struct Triangle {
    Vertex* v0;
    Vertex* v1;
    Vertex* v2;

    Triangle(Vertex* v0, Vertex* v1, Vertex* v2) {
        this->v0 = v0;
        this->v1 = v1;
        this->v2 = v2;
    }

    [[nodiscard]] glm::vec3 getNormal() const { return normalize(v0->normal + v1->normal + v2->normal); }
    [[nodiscard]] glm::vec3 getCenter() const { return (v0->position + v1->position + v2->position) / 3.0f; }

    [[nodiscard]] glm::vec3 getPhysicalNormal() const {
        return normalize(cross(v1->position - v0->position, v2->position - v0->position));
    }

    [[nodiscard]] float getArea() const {
        glm::vec3 e0 = v1->position - v0->position;
        glm::vec3 e1 = v2->position - v0->position;

        return length(cross(e0, e1)) / 2.0f;
    }
};

struct BoundingBox {
    glm::vec3 min, max;
};

/**
 * Static mesh class holds vertex data and indices for a mesh. Can draw itself using a material.
 * Does not have it's own transform as it is intended to be instanced - use draw(Transform* transform) to draw mesh at instance transform.
 */
class StaticMesh : public Drawable {
    GLuint VAO{};
    GLuint VBO[4]{};
    GLuint EBO{};
    Material* material;

    std::vector<unsigned int> indices;
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;

    BoundingBox boundingBox;
public:
    StaticMesh(const aiMesh* mesh, Material* material);

    StaticMesh(const StaticMesh& other)
        : Drawable(other),
          indices(other.indices),
          vertices(other.vertices),
          triangles(other.triangles), boundingBox(other.boundingBox) {
        material = new Material(*other.material);
        StaticMesh::init();
    }

    StaticMesh(StaticMesh&& other) noexcept
        : Drawable(std::move(other)),
          indices(std::move(other.indices)),
          vertices(std::move(other.vertices)),
          triangles(std::move(other.triangles)), boundingBox(other.boundingBox) {

        material = new Material(*other.material);
        StaticMesh::init();
    }

    StaticMesh& operator=(const StaticMesh& other) {
        if (this == &other)
            return *this;
        Drawable::operator =(other);
        indices = other.indices;
        vertices = other.vertices;
        triangles = other.triangles;
        material = new Material(*other.material);
        boundingBox = other.boundingBox;
        StaticMesh::init();
        return *this;
    }

    StaticMesh& operator=(StaticMesh&& other) noexcept {
        if (this == &other)
            return *this;
        indices = std::move(other.indices);
        vertices = std::move(other.vertices);
        triangles = std::move(other.triangles);
        material = new Material(*other.material);

        StaticMesh::init();

        return *this;
    }

    static std::vector<StaticMesh*> batchImport(const std::filesystem::path& path, Material* materialOverride = nullptr,
                                                Shader* shaderOverride = nullptr);

    void init() override;
    void draw() override;
    void draw(const Transform& transform) override;

    [[nodiscard]] BoundingBox getBoundingBox() const { return boundingBox; }
    [[nodiscard]] std::vector<unsigned> getIndices() const { return indices; }
    [[nodiscard]] GLuint getVAO() const { return VAO; }
    [[nodiscard]] GLuint getEBO() const { return EBO; }
    [[nodiscard]] Material* getMaterial() const { return material; }

    void setMaterial(Material* const newMaterial) { this->material = newMaterial; }

    [[nodiscard]] glm::vec3 getCenter() const;
    [[nodiscard]] float calculateVolume() const;
    [[nodiscard]] float calculateSurfaceArea() const;

    [[nodiscard]] std::vector<glm::vec3> calculateNormals() const;

    ~StaticMesh() override {
        glDeleteBuffers(4, VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

private:
    /**
     * Calculates the axis aligned bounding box of the mesh in local space.
     */
    BoundingBox calculateBoundingBox() const;

};
