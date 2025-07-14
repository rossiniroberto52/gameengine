
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader();
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    bool LoadFromString(const std::string& vertexSource, const std::string& fragmentSource);

    void Bind();
    void Unbind();
    GLuint GetProgramID() const { return m_programID; }
private:
    bool CompileShader(GLuint shaderID, const std::string& source);

    GLuint m_programID;
    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
};

#endif // SHADER_H
