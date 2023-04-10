#include "display.h"

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void _resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* display_create(int width, int height, char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return NULL;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, _resize_callback);

    return window;
}

int display_should_close(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}

void display_refresh(GLFWwindow* window) {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

float display_aspect(GLFWwindow* window) {
    int width;
    int height;
    glfwGetWindowSize(window, &width, &height);
    return (float)width / (float)height;
}

void display_free(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}