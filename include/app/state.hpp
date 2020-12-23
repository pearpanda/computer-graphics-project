#ifndef RG_STATE_HPP
#define RG_STATE_HPP

#include <GLFW/glfw3.h>

#include <app/constants.hpp>
#include <app/objects/Ball.hpp>
#include <app/objects/Camera.hpp>
#include <app/objects/Floor.hpp>
#include <app/objects/Lamp.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/light/lights.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <array>
#include <vector>

namespace app {

struct TimeState {
    float elapsed = 0.0f;
    float delta = 0.0f;

    void update();
};

struct CameraState {
    std::array<Camera*, 4> cameras{nullptr};
    std::array<rg::Surface*, 4> surfaces{nullptr};
    unsigned int active_camera = 0;
    bool multiple_cameras = true;

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

    Camera& get_active_camera();
    [[nodiscard]] const Camera& get_active_camera() const;

    ~CameraState();
};

struct MouseState {
    bool bound_mouse = false;
    bool used_mouse = false;
    float last_x = 0.0f;
    float last_y = 0.0f;

    float delta_x = 0.0f;
    float delta_y = 0.0f;
};

struct LightState {
    std::vector<rg::DirectionalLight>* directional{nullptr};
    std::vector<rg::PointLight>* point{nullptr};
    std::vector<rg::SpotLight>* spotlight{nullptr};

    ~LightState();
};

struct State {
    GLFWwindow* window = nullptr;
    unsigned int window_width = WINDOW_WIDTH;
    unsigned int window_height = WINDOW_HEIGHT;

    TimeState time_subsystem;
    CameraState camera_subsystem;
    MouseState mouse_subsystem;
    LightState light_subsystem;

    // The shader used to draw most objects
    rg::Shader* shader = nullptr;
    // The shader used to draw skyboxes
    rg::Shader* skybox_shader = nullptr;
    // The shader used to draw framebuffers
    rg::Shader* surface_shader = nullptr;
    // The shader used to draw light sources
    rg::Shader* light_shader = nullptr;

    rg::Skybox* skybox = nullptr;

    Ball* ball = nullptr;
    Lamp* lamp = nullptr;
    Floor* floor = nullptr;

#ifdef ENABLE_DEBUG
    rg::Model* debug_cube;
    rg::Shader* debug_shader;
#endif // ENABLE_DEBUG

    ~State();
};

extern State* state;

} // namespace app

#endif // RG_STATE_HPP
