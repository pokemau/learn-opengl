#ifndef SLOMMGL_GL_H
#define SLOMMGL_GL_H

#include "../../vendor/glad/include/glad/glad.h"
#include "../../vendor/glfw-3.4/include/GLFW/glfw3.h"
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

#include <iostream>


class GL {
    GLFWwindow *_window;

public:
    void Init(int SCREEN_W, int SCREEN_H, const std::string& title);
    GLFWwindow *GetWindowPointer();
    bool WindowIsOpen();
    void SetWindowShouldClose(bool val);
    void SwapBuffersPollEvents();
    static void Cleanup();
    void ProcessInput();
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};
#endif //SLOMMGL_GL_H
