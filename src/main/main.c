#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../display/display.h"

int main(int argc, char** argv) {
    GLFWwindow* window = display_create(800, 500, "Boat");
    while (!display_should_close(window)) {
        display_refresh(window);
    }
    display_free(window);
    return 0;
}