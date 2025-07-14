#pragma once

#include "Window.h"
#include "Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdint>

struct Vertex {
    glm::vec2 position;
    glm::vec4 color;
};

struct Batch{
    uint32_t vertexOffset;
    uint32_t indexOffset;
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    uint32_t texture = 0;
};

class RenderSystem {
    public:
        static const uint32_t MAX_VERTICES = 10000;
        static const uint32_t MAX_INDICES = 30000;

        RenderSystem(Window* window);
        ~RenderSystem();
        
        bool Initialize();
        void Shutdown();

        void Clear(float r, float g, float b, float a);
        void RenderBegin();
        void RenderEnd();
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        void UpdateViewport(int width, int height);
    
    private:
        void Flush();
        void InitializeBatchRenderer();

        Window* m_window;
        Shader m_basicShader;
        glm::mat4 m_projection;

        //batch rendering data
        std::vector<Vertex> m_vertexData;
        std::vector<uint32_t> m_indexData;
        std::vector<Batch> m_batches;
        Batch* m_currentBatch = nullptr;

        //openGL objects
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ibo = 0;
};
   