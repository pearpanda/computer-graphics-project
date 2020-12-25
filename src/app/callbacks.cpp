#include <app/init.hpp>

#include <glad/glad.h>

namespace app {

void bindCallbacks() {
    // Callbacks
    // ---------
    glfwSetFramebufferSizeCallback(state->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(state->window, mouse_callback);
    glfwSetKeyCallback(state->window, switches_callback);
}

void switches_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    auto& camera_subsystem = state->camera_subsystem;
    auto& multiple_cameras = camera_subsystem.multiple_cameras;
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        multiple_cameras = !multiple_cameras;

    auto& active_camera = camera_subsystem.active_camera;
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        active_camera = 0;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        active_camera = 1;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        active_camera = 2;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        active_camera = 3;

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        if (!state->mouse_subsystem.bound_mouse) {
            glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            state->mouse_subsystem.bound_mouse = true;
        } else {
            glfwSetInputMode(state->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            state->mouse_subsystem.bound_mouse = false;
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        state->enable_simulation = !state->enable_simulation;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
    state->window_width = width;
    state->window_height = height;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto x_position = static_cast<float>(xpos);
    auto y_position = static_cast<float>(ypos);

    auto& mouse_subsystem = state->mouse_subsystem;
    if (!mouse_subsystem.used_mouse) {
        mouse_subsystem.last_x = x_position;
        mouse_subsystem.last_y = y_position;
        mouse_subsystem.used_mouse = true;
    }

    mouse_subsystem.delta_x = x_position - mouse_subsystem.last_x;
    mouse_subsystem.delta_y = y_position - mouse_subsystem.last_y;

    mouse_subsystem.last_x = x_position;
    mouse_subsystem.last_y = y_position;

    auto& camera_subsystem = state->camera_subsystem;
    camera_subsystem.delta_yaw +=
            camera_subsystem.camera_sensitivity * (-mouse_subsystem.delta_x);
    camera_subsystem.delta_pitch +=
            camera_subsystem.camera_sensitivity * (-mouse_subsystem.delta_y);
}

} // namespace app
