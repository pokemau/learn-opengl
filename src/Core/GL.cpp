#include "GL.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void GL::Init(int SCREEN_W, int SCREEN_H, const std::string &title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window =
        glfwCreateWindow(SCREEN_W, SCREEN_H, title.c_str(), nullptr, nullptr);
    if (_window == nullptr) {
        std::cout << "FAILED TO CREATE WINDOW" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);

    // disable vsync
    //    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "FAILED TO INITIALIZE GLAD!" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow *GL::GetWindowPointer() { return _window; }

bool GL::WindowIsOpen() { return !glfwWindowShouldClose(_window); }

void GL::SetWindowShouldClose(bool val) {
    glfwSetWindowShouldClose(_window, val);
}

void GL::SwapBuffersPollEvents() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void GL::Cleanup() { glfwTerminate(); }

void GL::ProcessInput() {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        SetWindowShouldClose(true);
    }
}
void GL::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
