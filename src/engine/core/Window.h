#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    void Update();
    void Render();
    GLFWwindow* GetGLFWWindow() const { return m_window; }
private:
    GLFWwindow* m_window;
};
#endif // WINDOW_H
