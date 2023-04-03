#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* display_create(int width, int height, char* title);

int display_should_close(GLFWwindow* window);

void display_refresh(GLFWwindow* window);

void display_free(GLFWwindow* window);