#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return -1;
    }

    // Criar uma janela
    GLFWwindow* window = glfwCreateWindow(800, 600, "Engine2D", NULL, NULL);
    if (!window) {
        std::cerr << "Erro ao criar janela" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Definir a janela como contexto atual
    glfwMakeContextCurrent(window);

    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Definir a cor de fundo
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Limpar a janela
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenhar algo (por exemplo, um triângulo)
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.5f);
        glEnd();

        // Trocar buffers
        glfwSwapBuffers(window);

        // Processar eventos
        glfwPollEvents();
    }

    // Encerrar GLFW
    glfwTerminate();

    return 0;
}
