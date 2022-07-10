#include "StaticMesh.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Util.hpp"

StaticMesh::StaticMesh(aiMesh* const mesh) {
    indices.reserve(3 * mesh->mNumFaces);
    vertices.reserve(mesh->mNumVertices);
    triangles.reserve(mesh->mNumFaces);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.emplace_back(i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals())
            vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                           mesh->mNormals[i].z);
        if (mesh->HasVertexColors(0))
            vertices[i].colour = glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g,
                                           mesh->mColors[0][i].b);
        if (mesh->HasTextureCoords(0))
            vertices[i].uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        auto normal = glm::vec3(0);
        aiVector3D e1, e2;
        
        e1 = mesh->mVertices[mesh->mFaces[i].mIndices[1]] - mesh->mVertices[mesh->mFaces[i].mIndices[0]];
        e2 = mesh->mVertices[mesh->mFaces[i].mIndices[2]] - mesh->mVertices[mesh->mFaces[i].mIndices[0]];

        Vertex* v0 = &vertices[mesh->mFaces[i].mIndices[0]];
        Vertex* v1 = &vertices[mesh->mFaces[i].mIndices[1]];
        Vertex* v2 = &vertices[mesh->mFaces[i].mIndices[2]];

        Triangle t(v0, v1, v2);
        triangles.emplace_back(t);
        
        glm::vec3 edge1 = glm::vec3(e1.x, e1.y, e1.z);
        glm::vec3 edge2 = glm::vec3(e2.x, e2.y, e2.z);

        normal = cross(edge1, edge2);
        normal = normalize(normal);

        if (!mesh->HasNormals()) {
            vertices[mesh->mFaces[i].mIndices[0]].normal += normal;
            vertices[mesh->mFaces[i].mIndices[1]].normal += normal;
            vertices[mesh->mFaces[i].mIndices[2]].normal += normal;
        }

        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.emplace_back(mesh->mFaces[i].mIndices[j]);
    }

    indices.shrink_to_fit();

    for (auto& vertex : vertices) vertex.normal = normalize(vertex.normal);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(4, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colour)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

}

BoundingBox StaticMesh::getModelSpaceBoundingBox() const {
    glm::vec3 min, max;
    min = max = vertices[0].position;

    for (const auto& vertex : vertices) {
        if (vertex.position.x < min.x) { min.x = vertex.position.x; }
        if (vertex.position.y < min.y) { min.y = vertex.position.y; }
        if (vertex.position.z < min.z) { min.z = vertex.position.z; }

        if (vertex.position.x > max.x) { max.x = vertex.position.x; }
        if (vertex.position.y > max.y) { max.y = vertex.position.y; }
        if (vertex.position.z > max.z) { max.z = vertex.position.z; }
    }

    return {min, max};
}

void StaticMesh::applyTransform(Camera* camera, Transform instanceTransform, Shader* shader) const {
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE,
                       &instanceTransform.getModelMatrix()[0][0]);
    glBindVertexArray(0);
}

std::vector<glm::vec3> StaticMesh::calculateNormals() const {
    std::vector<glm::vec3> normals(vertices.size());

    for (const auto triangle : triangles) {
        glm::vec3 e1 = triangle.v1->position - triangle.v0->position;
        glm::vec3 e2 = triangle.v2->position - triangle.v0->position;

        glm::vec3 normal = cross(e1, e2);
        normal = normalize(normal);

        normals[triangle.v0->index] += normal;
        normals[triangle.v1->index] += normal;
        normals[triangle.v2->index] += normal;
    }
    for (auto& normal : normals) normal = normalize(normal);

    return normals;
}

// todo remove
glm::vec3 StaticMesh::getCenter() const {
    auto center = glm::vec3(0);
    for (const auto& vertex : vertices) center += vertex.position;
    center /= vertices.size();

    return center;
}

float StaticMesh::calculateVolume() const {
    const glm::vec3 center = getCenter();
    float volume = 0;
    for (const auto triangle : triangles) volume += Util::tetrahedronVolume(triangle, center);

    return volume;
}

float StaticMesh::calculateSurfaceArea() const {
    float area = 0;
    for (const auto triangle : triangles) area += triangle.getArea();

    return area;
}
