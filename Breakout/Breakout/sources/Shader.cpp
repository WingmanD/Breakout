#define STB_IMAGE_IMPLEMENTATION

#include "Shader.hpp"

#include <fstream>
#include <iostream>

#include <glad/glad.h>


Shader::Shader(std::filesystem::path shaderPath) { init(shaderPath); }

void Shader::init(std::filesystem::path shaderPath) {
    this->path = shaderPath;
    ID = glCreateProgram();

    GLuint vertexShader, fragmentShader, geometryShader;

    if (readShader(path, VERTEX, vertexShader))
        glAttachShader(ID, vertexShader);
    if (readShader(path, GEOMETRY, geometryShader))
        glAttachShader(ID, geometryShader);
    if (readShader(path, FRAGMENT, fragmentShader))
        glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    checkCompilerErrors(ID, PROGRAM);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    GLuint viewBlockIndex = glGetUniformBlockIndex(ID, "ViewData");

    glUniformBlockBinding(ID, viewBlockIndex, 0);
}


bool Shader::readShader(std::filesystem::path shaderPath, ShaderType type, GLuint& shaderID) const {
    GLuint shader;
    std::string shaderCode;
    const char* shaderSource;

    switch (type) {
    case VERTEX:
        shaderPath += ".vert";
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
    case FRAGMENT:
        shaderPath += ".frag";
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case GEOMETRY:
        shaderPath += ".geom";
        shader = glCreateShader(GL_GEOMETRY_SHADER);
        break;

    case PROGRAM:
    case COMPUTE: return false;
    }

    if (!exists(shaderPath)) {
        std::cerr << "Shader file " << shaderPath << " does not exist" << std::endl;
        return false;
    }

    try {
        std::ifstream shaderFile(shaderPath);
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::stringstream ss;
        ss << shaderFile.rdbuf();
        shaderFile.close();

        shaderCode = ss.str();
        shaderSource = shaderCode.c_str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << shaderPath << std::endl;
        std::cerr << e.what() << std::endl;
    }

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    checkCompilerErrors(shader, type);

    shaderID = shader;
    return true;
}


void Shader::checkCompilerErrors(unsigned int shader, ShaderType type) {
    int success;
    char infolog[1024];
    if (type != PROGRAM) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infolog);
            std::cout << "Error compiling shader type " << ShaderTypeStrings[type] << ": " << infolog << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infolog);
            std::cout << "Error linking shader type " << ShaderTypeStrings[type] << ": " << infolog << std::endl;
        }
    }
}

void Shader::use() const {
    glUseProgram(ID);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::setUniform(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setUniform(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
