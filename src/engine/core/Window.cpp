#include "Window.h"
Window::Window(int width, int height, const char* title) {
    glfwInit();
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
}
Window::~Window() {
    glfwTerminate();
}
void Window::Update() {
    glfwPollEvents();
}
void Window::Render() {
    glfwSwapBuffers(m_window);
}