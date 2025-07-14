#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader() : m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0) {
    // Default constructor
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) : Shader() {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);
    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        std::cerr << "Failed to open shader files: " << vertexShaderPath << ", " << fragmentShaderPath << std::endl;
        m_programID = 0;
        return;
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    vShaderFile.close();
    fShaderFile.close();

    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    if (!CompileShader(m_vertexShaderID, vertexCode) || !CompileShader(m_fragmentShaderID, fragmentCode)) {
        m_programID = 0;
        return;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        m_programID = 0;
    }
}

Shader::~Shader() {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
    if (m_vertexShaderID != 0) {
        glDeleteShader(m_vertexShaderID);
    }
    if (m_fragmentShaderID != 0) {
        glDeleteShader(m_fragmentShaderID);
    }
}

void Shader::Bind() {
    if (m_programID != 0) {
        glUseProgram(m_programID);
    }
}

void Shader::Unbind() {
    glUseProgram(0);
}

bool Shader::CompileShader(GLuint shaderID, const std::string& source) {
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, NULL);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::LoadFromString(const std::string& vertexSource, const std::string& fragmentSource) {
    if (m_programID != 0) {
        glDeleteProgram(m_programID);
    }
    if (m_vertexShaderID != 0) {
        glDeleteShader(m_vertexShaderID);
    }
    if (m_fragmentShaderID != 0) {
        glDeleteShader(m_fragmentShaderID);
    }

    m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    if (!CompileShader(m_vertexShaderID, vertexSource) || !CompileShader(m_fragmentShaderID, fragmentSource)) {
        m_programID = 0;
        return false;
    }

    m_programID = glCreateProgram();
    glAttachShader(m_programID, m_vertexShaderID);
    glAttachShader(m_programID, m_fragmentShaderID);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        m_programID = 0;
        return false;
    }
    return true;
}
