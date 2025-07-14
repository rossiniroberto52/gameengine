#include "Render.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::RenderSystem(Window* window) : m_window(window) {
    Initialize();
}

RenderSystem::~RenderSystem() {
    Shutdown();
}

bool RenderSystem::Initialize(){
    glfwMakeContextCurrent(m_window->GetGLFWWindow());
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    //set OpenGL for 2d rendering
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int width, height;
    glfwGetFramebufferSize(m_window->GetGLFWWindow(), &width, &height);
    glViewport(0, 0, width, height);

    //init projection matrix
    m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);

    //Initialize basic shader
    if (!m_basicShader.LoadFromString(
        // Vertex shader
        "#version 330 core\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "uniform mat4 projection;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    gl_Position = projection * vec4(position.xy, 0.0, 1.0);\n"
        "    fragColor = color;\n"
        "}\n",
        
        // Fragment shader
        "#version 330 core\n"
        "in vec4 fragColor;\n"
        "out vec4 outputColor;\n"
        "void main() {\n"
        "    outputColor = fragColor;\n"
        "}\n")) {
        return false;
    }

    InitializeBatchRenderer();
    std::cout << "RenderSystem initialized successfully." << std::endl;
    return true;
}

void RenderSystem::Shutdown() {
    m_basicShader.Unbind();
    // ShutdownBatchRenderer(); // Removed because function is undefined
}

void RenderSystem::Clear(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::RenderEnd(){
    Flush();
    m_basicShader.Unbind();
}

void RenderSystem::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color){
    if(m_vertexData.size() + 4 > MAX_VERTICES || m_indexData.size() + 6 > MAX_INDICES || m_currentBatch->texture != 0){
        Flush();
    }
    //calc vertices
    float left  = position.x;
    float right = position.x + size.x;
    float top = position.y;
    float bottom = position.y + size.y;
    //add vertices
    m_vertexData.push_back(Vertex{glm::vec2(left, top), color});
    m_vertexData.push_back(Vertex{glm::vec2(right, top), color});
    m_vertexData.push_back(Vertex{glm::vec2(right, bottom), color});
    m_vertexData.push_back(Vertex{glm::vec2(left, bottom), color});

    //add indices
    uint32_t offset = m_currentBatch->vertexCount;
    m_indexData.push_back(offset + 0);
    m_indexData.push_back(offset + 1);
    m_indexData.push_back(offset + 2);
    m_indexData.push_back(offset + 2);
    m_indexData.push_back(offset + 3);
    m_indexData.push_back(offset + 0);

    m_currentBatch->vertexCount += 4;
    m_currentBatch->indexCount += 6;
}

void RenderSystem::Flush() {
    if(m_currentBatch->indexCount == 0) return;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, m_currentBatch->vertexOffset * sizeof(Vertex), m_currentBatch->vertexCount * sizeof(Vertex), &m_vertexData[m_currentBatch->vertexOffset]);
    //Update index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_currentBatch->indexOffset * sizeof(uint32_t), m_currentBatch->indexCount * sizeof(uint32_t), &m_indexData[m_currentBatch->indexOffset]);

    //draw
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_currentBatch->indexCount, GL_UNSIGNED_INT, (void*)(m_currentBatch->indexOffset * sizeof(uint32_t)));

    //prepare for next batch
    m_batches.emplace_back();
    m_currentBatch = &m_batches.back();
    m_currentBatch->vertexOffset = m_vertexData.size();
    m_currentBatch->indexOffset = m_indexData.size();
}

void RenderSystem::InitializeBatchRenderer(){
    // generate vao, vbo, ibo
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);

    glBindVertexArray(m_vao);

    //setup VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

    //vertex attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    //color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    //Setup IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void RenderSystem::UpdateViewport(int width, int height){
    glViewport(0, 0, width, height);
    m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
}
