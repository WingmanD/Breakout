#pragma once

#include <iostream>
#include <map>
#include <glm/vec3.hpp>

#include "Shader.hpp"
#include "assimp/material.h"
#include <stb_image.h>

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
        
        if (data) {
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
        else throw std::runtime_error("Failed to load texture: " + path.string());
    }
};

class Material {
    std::string name;

    Shader* shader;

    glm::vec3 ambient = {0.1, 0.1, 0.1};
    glm::vec3 diffuse = {0.5, 0.5, 0.5};
    glm::vec3 specular = {1, 1, 1};
    glm::vec3 emissive = {0, 0, 0};

    float shininess = 5;
    float opacity = 1;

    std::map<TextureType, Texture2D> textures;

public:
    explicit Material(Shader* shader, const aiMaterial* material, const std::filesystem::path& scenePath = "");
    Material(Shader* shader) : shader(shader) {}

    //todo shader getter and setter with binding
    void apply();
void applyTextures();
    
    void setup();

    void setTextureMap(TextureType type, const std::filesystem::path& path);
    
    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] glm::vec3 getAmbientColor() const { return ambient; }
    [[nodiscard]] glm::vec3 getDiffuseColor() const { return diffuse; }
    [[nodiscard]] glm::vec3 getSpecularColor() const { return specular; }
    [[nodiscard]] glm::vec3 getEmissiveColor() const { return emissive; }
    [[nodiscard]] float getShininess() const { return shininess; }
    [[nodiscard]] float getOpacity() const { return opacity; }

    //todo send to shader as well
    void setName(const std::string& newName) { this->name = newName; }
    void setAmbient(const glm::vec3& newAmbient) {
        this->ambient = newAmbient;
    }
    void setDiffuse(const glm::vec3& newDiffuse) {
        this->diffuse = newDiffuse;
        shader->use();
        glUniform3fv(glGetUniformLocation(shader->ID, "diffuseColor"), 1, &diffuse[0]);
    }
    void setSpecular(const glm::vec3& newSpecular) { this->specular = newSpecular; }
    void setEmissive(const glm::vec3& newEmissive) { this->emissive = newEmissive; }
    void setShininess(float newShininess) { this->shininess = newShininess; }
    void setOpacity(float newOpacity) { this->opacity = newOpacity; }

private:
    void loadTextures(const aiMaterial* material, const std::filesystem::path& textureDirPath);
    void setupTexture( Texture2D& texture);
    
};
