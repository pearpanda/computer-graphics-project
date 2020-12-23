#include <app/loop.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <app/Node.hpp>
#include <app/state.hpp>
#include <rg/renderer/render.hpp>

namespace app {

namespace {

void iteration();

// Used to process continuous input
void processInput();
void togglePressed(int key, bool& value);

void draw();
void drawScene();
void drawMultipleCameras();
void drawSingleCamera();
void swapBuffers();

void update();
void updateTime();
void updateCameras();
void updateObjects();

void pollEvents();

} // namespace

void loop() {
    while (!glfwWindowShouldClose(app::state->window))
        iteration();
}

namespace {

void iteration() {
    processInput();
    update();
    draw();
    update();
    pollEvents();
}

void processInput() {
    auto& camera_behaviour = state->camera_subsystem.behaviour;
    togglePressed(GLFW_KEY_W, camera_behaviour.move_forward);
    togglePressed(GLFW_KEY_A, camera_behaviour.move_left);
    togglePressed(GLFW_KEY_S, camera_behaviour.move_backward);
    togglePressed(GLFW_KEY_D, camera_behaviour.move_right);
}

void togglePressed(int key, bool& value) {
    if (glfwGetKey(state->window, key) == GLFW_PRESS)
        value = true;
    else
        value = false;
}

void draw() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bool multiple_cameras = state->camera_subsystem.multiple_cameras;
    if (multiple_cameras)
        drawMultipleCameras();
    else
        drawSingleCamera();

    swapBuffers();
}

void update() {
    updateTime();
    updateCameras();
    updateObjects();
}

void drawScene(const Camera& camera, const rg::Surface& surface) {
    const auto& shader = state->shader;
    const auto& skybox_shader = state->skybox_shader;

    const auto& ball = state->ball;
    const auto& skybox = state->skybox;

    rg::clear(surface);
    rg::render(
            *shader, *ball->model, camera.get_view(), surface,
            ball->get_transform(), 64.0f, *state->light_subsystem.directional,
            *state->light_subsystem.point, *state->light_subsystem.spotlight);

#ifdef ENABLE_DEBUG
    for (auto light : *state->light_subsystem.point) {
        rg::Transform transform;
        transform.position = light.position;
        rg::render(*state->debug_shader, *state->debug_cube, camera.get_view(),
                   surface, transform);
    }
    for (auto light : *state->light_subsystem.spotlight) {
        rg::Transform transform;
        transform.position = light.position;
        transform.orientation = glm::quat(glm::vec3{0.7f, 0.7f, 0.7f});
        rg::render(*state->debug_shader, *state->debug_cube, camera.get_view(),
                   surface, transform);
    }
#endif // ENABLE_DEBUG

    rg::render(*skybox_shader, *skybox, camera.get_view(), surface);
}

void drawMultipleCameras() {
    const auto& surface_shader = state->surface_shader;
    const auto& cameras = state->camera_subsystem.cameras;
    const auto& surfaces = state->camera_subsystem.surfaces;

    // Draw objects as seen from each camera to the camera's own surface
    // -----------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    for (unsigned int i = 0; i < 4; ++i)
        drawScene(*cameras[i], *surfaces[i]);

    // Draw surfaces to the screen
    // ---------------------------
    rg::clear();
    glDisable(GL_DEPTH_TEST);
    for (unsigned int i = 0; i < 4; ++i)
        rg::render(*surface_shader, *surfaces[i], i);
}

void drawSingleCamera() {
    const auto& surface_shader = state->surface_shader;

    const auto& active_camera = state->camera_subsystem.active_camera;
    const auto& camera = state->camera_subsystem.cameras[active_camera];
    const auto& surface = state->camera_subsystem.surfaces[active_camera];

    glEnable(GL_DEPTH_TEST);
    drawScene(*camera, *surface);

    glDisable(GL_DEPTH_TEST);
    rg::clear();
    rg::render(*surface_shader, *surface);
}

void updateCameras() {
    const auto& active_camera = state->camera_subsystem.active_camera;
    const auto& camera = state->camera_subsystem.cameras[active_camera];

    auto front = camera->get_direction();
    auto right = camera->get_right();
    auto delta_time = state->time_subsystem.delta;
    auto speed = state->camera_subsystem.camera_speed * delta_time;

    if (state->camera_subsystem.behaviour.move_forward)
        camera->move(speed * front);
    if (state->camera_subsystem.behaviour.move_backward)
        camera->move(-speed * front);
    if (state->camera_subsystem.behaviour.move_right)
        camera->move(speed * right);
    if (state->camera_subsystem.behaviour.move_left)
        camera->move(-speed * right);

    auto& delta_yaw = state->camera_subsystem.delta_yaw;
    auto& delta_pitch = state->camera_subsystem.delta_pitch;
    camera->rotate(delta_yaw, delta_pitch);
    delta_yaw = 0.0f;
    delta_pitch = 0.0f;
}

void updateObjects() {
}

void pollEvents() {
    glfwPollEvents();
}

void updateTime() {
    state->time_subsystem.update();
}

void swapBuffers() {
    glfwSwapBuffers(app::state->window);
}

} // namespace

} // namespace app
