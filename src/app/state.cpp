#include <app/state.hpp>

namespace app {

rg::Camera& CameraState::get_active_camera() {
    return *cameras[active_camera];
}

const rg::Camera& CameraState::get_active_camera() const {
    return *cameras[active_camera];
}

CameraState::~CameraState() {
    for (auto& camera : cameras) {
        delete camera;
        camera = nullptr;
    }

    for (auto& surface : surfaces) {
        delete surface;
        surface = nullptr;
    }
}

LightState::~LightState() {
    delete directional;
    delete point;
    delete spotlight;
    directional = nullptr;
    point = nullptr;
    spotlight = nullptr;
}

State::~State() {
    // Objects
    // -------
    delete ball;

    // Skybox
    // ------
    delete skybox;

    // Shaders
    // -------
    delete shader;
    delete skybox_shader;
    delete surface_shader;

#ifdef ENABLE_DEBUG
    delete debug_cube;
    delete debug_shader;
#endif
}

void TimeState::update() {
    auto time = static_cast<float>(glfwGetTime());
    delta = time - elapsed;
    elapsed = time;
}

} // namespace app
