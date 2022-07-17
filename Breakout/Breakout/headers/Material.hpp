#pragma once

#include <iostream>
#include <map>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "assimp/material.h"
#include <stb_image.h>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>

enum TextureType {
    DIFFUSE,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    HEIGHT,
    NORMAL,
    SHININESS,
    OPACITY,
};

const std::string TextureTypeNames[] = {
    "diffuseMap",
    "specularMap",
    "ambientMap",
    "emissiveMap",
    "heightMap",
    "normalMap",
    "shininessMap",
    "opacityMap",
};

struct Texture2D {
    GLuint ID{};
    int width = 0, height = 0, channels = 0;
    std::unique_ptr<unsigned char*> data;
    TextureType type = DIFFUSE;

    Texture2D() = default;

    Texture2D(const std::filesystem::path& path, TextureType type) {
        this->type = type;

        data = std::make_unique<unsigned char*>(stbi_load(path.string().c_str(), &width, &height, &channels, 0));

        if (!data || width == 0 || height == 0)
            std::cerr << "Failed to load texture: " << path << std::endl;

        glGenTextures(1, &ID);

        glActiveTexture(GL_TEXTURE0 + type);

        glBindTexture(GL_TEXTURE_2D, ID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, *data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

/**
 *  Material class handles the loading of textures and material parameters.
 *  It can be initialized from file or manually.
 *  Passes data to shader programs.
 */
class Material {
    std::string name;

    Shader* shader;

    glm::vec3 ambient = {0.1, 0.1, 0.1};
    glm::vec3 diffuse = {0.5, 0.5, 0.5};
    glm::vec3 specular = {1, 1, 1};
    glm::vec3 emissive = {0, 0, 0};

    glm::vec2 textureScale = {1, 1};
    glm::vec2 textureOffset = {0, 0};

    float shininess = 5;
    float opacity = 1;

    std::map<TextureType, Texture2D*> textures;

public:
    explicit Material(Shader* shader, const aiMaterial* material, const std::filesystem::path& scenePath = "");

    Material(Shader* shader);

    Material(const Material& other) = default;

    Material(Material&& other) noexcept
        : name(std::move(other.name)),
          shader(new Shader(*other.shader)),
          ambient(other.ambient),
          diffuse(other.diffuse),
          specular(other.specular),
          emissive(other.emissive),
          shininess(other.shininess),
          opacity(other.opacity),
          textures(other.textures) {}

    Material& operator=(const Material& other) {
        if (this == &other)
            return *this;
        name = other.name;
        shader = new Shader(*other.shader);
        ambient = other.ambient;
        diffuse = other.diffuse;
        specular = other.specular;
        emissive = other.emissive;
        shininess = other.shininess;
        opacity = other.opacity;
        textures = other.textures;
        return *this;
    }

    Material& operator=(Material&& other) noexcept {
        if (this == &other)
            return *this;
        name = other.name;
        shader = new Shader(*other.shader);
        ambient = other.ambient;
        diffuse = other.diffuse;
        specular = other.specular;
        emissive = other.emissive;
        shininess = other.shininess;
        opacity = other.opacity;
        textures = other.textures;
        return *this;
    }

    void apply();
    void applyTextures();

    void setup();

    /**
     * Set texture in shader for given texture type.
     */
    void setTextureMap(TextureType type, const std::filesystem::path& path);
    
    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] Shader* getShader() const { return shader; }
    [[nodiscard]] glm::vec3 getAmbientColor() const { return ambient; }
    [[nodiscard]] glm::vec3 getDiffuseColor() const { return diffuse; }
    [[nodiscard]] glm::vec3 getSpecularColor() const { return specular; }
    [[nodiscard]] glm::vec3 getEmissiveColor() const { return emissive; }
    [[nodiscard]] float getShininess() const { return shininess; }
    [[nodiscard]] float getOpacity() const { return opacity; }
    [[nodiscard]] glm::vec2 getTextureScale() const { return textureScale; }
    [[nodiscard]] glm::vec2 getTextureOffset() const { return textureOffset; }


    void setName(const std::string& newName) { this->name = newName; }

    void setAmbient(const glm::vec3& newAmbient) {
        this->ambient = newAmbient;
    }

    void setDiffuse(const glm::vec3& newDiffuse) {
        this->diffuse = newDiffuse;
    }

    void setSpecular(const glm::vec3& newSpecular) {
        this->specular = newSpecular;
    }

    void setEmissive(const glm::vec3& newEmissive) {
        this->emissive = newEmissive;
    }

    void setShininess(float newShininess) {
        this->shininess = newShininess;
    }

    void setOpacity(float newOpacity) {
        this->opacity = newOpacity;
    }

    void setTextureScale(const glm::vec2& newTextureScale) {
        this->textureScale = newTextureScale;
    }

    void setTextureOffset(const glm::vec2& newTextureOffset) {
        textureOffset = newTextureOffset;
    }

private:
    /**
     * Load textures from asset file.
     */
    void loadTextures(const aiMaterial* material, const std::filesystem::path& textureDirPath);

    /**
     * Set up texture uniforms.
     */
    void setupTexture(Texture2D* texture);

};
