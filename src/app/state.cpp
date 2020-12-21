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

State::~State() {
    // Skybox
    // ------
    delete skybox;

    // Shaders
    // -------
    delete shader;
    delete skybox_shader;
    delete surface_shader;
}

void TimeState::update() {
    auto time = static_cast<float>(glfwGetTime());
    delta = time - elapsed;
    elapsed = time;
}

} // namespace app
