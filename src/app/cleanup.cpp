#include <app/cleanup.hpp>

#include <app/state.hpp>

void app::cleanup() {
    GLFWwindow* window = state->window;
    state->window = nullptr;

    delete state;
    glfwDestroyWindow(window);
    glfwTerminate();
}
