#ifndef RG_STATE_HPP
#define RG_STATE_HPP

#include <GLFW/glfw3.h>

#include <app/constants.hpp>
#include <rg/renderer/camera/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <array>

namespace app {

struct TimeState {
    float elapsed = 0.0f;
    float delta = 0.0f;

    void update();
};

struct CameraState {
    std::array<rg::Camera*, 4> cameras{nullptr};
    std::array<rg::Surface*, 4> surfaces{nullptr};
    unsigned int active_camera = 0;
    bool multiple_cameras = false;

    float camera_speed = CAMERA_SPEED;
    float camera_sensitivity = CAMERA_SENSITIVITY;
    float delta_yaw = 0.0f;
    float delta_pitch = 0.0f;

    struct Behaviour {
        bool move_forward = false;
        bool move_backward = false;
        bool move_left = false;
        bool move_right = false;
    };

    Behaviour behaviour;

    rg::Camera& get_active_camera();
    [[nodiscard]] const rg::Camera& get_active_camera() const;

    ~CameraState();
};

struct MouseState {
    bool used_mouse = false;
    float last_x = 0.0f;
    float last_y = 0.0f;

    float delta_x = 0.0f;
    float delta_y = 0.0f;
};

struct State {
    GLFWwindow* window = nullptr;
    unsigned int window_width = WINDOW_WIDTH;
    unsigned int window_height = WINDOW_HEIGHT;

    TimeState time_subsystem;
    CameraState camera_subsystem;
    MouseState mouse_subsystem;

    // The shader used to render most objects
    rg::Shader* shader = nullptr;
    rg::Shader* skybox_shader = nullptr;
    // The shader used to draw framebuffers
    rg::Shader* surface_shader = nullptr;

    rg::Skybox* skybox;

    rg::Model* backpack;

    ~State();
};

extern State* state;

} // namespace app

#endif // RG_STATE_HPP
