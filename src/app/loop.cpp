#include <app/loop.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <app/state.hpp>
#include <rg/renderer/render.hpp>

#include <spdlog/spdlog.h>

namespace app {

namespace {

void iteration();

// Used to process continuous input
void processInput();
void togglePressed(int key, bool& value);

void draw();
void drawScene(const Camera& camera, const rg::Surface& surface);
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

    const auto& shader = *state->shader;
    const auto& directional_lights = *state->light_subsystem.directional;
    const auto& point_lights = *state->light_subsystem.point;
    const auto& spotlights = *state->light_subsystem.spotlight;

    // Lights
    // ------
    // Light initialization is placed here in order to speed up execution.
    shader.bind();
    shader.set_int("active_directional_lights",
                   static_cast<int>(directional_lights.size()));
    shader.set_int("active_point_lights",
                   static_cast<int>(point_lights.size()));
    shader.set_int("active_spotlights", static_cast<int>(spotlights.size()));
    for (unsigned int i = 0; i < directional_lights.size(); ++i)
        shader.set("directional_lights[" + std::to_string(i) + "]",
                   directional_lights[i]);
    for (unsigned int i = 0; i < point_lights.size(); ++i)
        shader.set("point_lights[" + std::to_string(i) + "]", point_lights[i]);
    for (unsigned int i = 0; i < spotlights.size(); ++i)
        shader.set("spotlights[" + std::to_string(i) + "]", spotlights[i]);
    shader.unbind();

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
    const auto& light_shader = state->light_shader;

    state->lamp->placeLight();

    const auto& view = camera.get_view();
    shader->set(view);
    shader->set("camera.position", view.position);
    shader->set("camera.direction", view.direction);

    glm::mat4 view_matrix = glm::mat4{glm::mat3{view.get_view_matrix()}};
    skybox_shader->set("view_matrix", view_matrix);
    skybox_shader->set("projection_matrix", view.get_projection_matrix());

    light_shader->set(view);

    surface.bind();
    rg::clear(surface);

    // Ball
    // ----
    const auto& ball = state->ball;
    rg::render(*shader, *ball->model, ball->transform, 32.0f);

    // Floor
    // -----
    const auto& floor = state->floor;
    rg::Transform tile_transform = floor->transform;
    glm::vec3 forward = tile_transform.get_forward_vector();
    glm::vec3 right = tile_transform.get_right_vector();
    glm::vec3 root = tile_transform.position;
    for (int i = -floor->width; i <= floor->width; ++i) {
        glm::vec3 dr = static_cast<float>(i) * right;
        for (int j = -floor->height; j <= floor->height; ++j) {
            glm::vec3 df = static_cast<float>(j) * forward;
            tile_transform.position = root + df + dr;

            rg::render(*shader, *state->floor->tile, tile_transform, 12.0f);
        }
    }

    // Lamp
    // ----
    const auto& lamp = state->lamp;
    rg::render(*shader, *lamp->base, lamp->transform, 64.0f);
    rg::render(*shader, *lamp->frame, lamp->transform, 64.0f);
    light_shader->bind();
    light_shader->set("light_color", lamp->get_color());
    rg::render(*light_shader, *lamp->source, lamp->transform);

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

    // Skybox
    // ------
    const auto& skybox = state->skybox;
    rg::render(*skybox_shader, *skybox);

    surface.unbind();
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
    for (unsigned int i = 0; i < 4; ++i) {
        rg::render(*surface_shader, *surfaces[i], i);
    }
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
    auto& spotlight = state->light_subsystem.spotlight->front();
    const auto& active_camera = state->camera_subsystem.active_camera;
    const auto& camera = *state->camera_subsystem.cameras[active_camera];
    spotlight.position = camera.get_position();
    spotlight.direction = camera.get_direction();

    static constexpr glm::vec3 g = glm::vec3{0.0f, -9.81f, 0.0f};
    float delta = state->time_subsystem.delta;
    if (state->enable_simulation) {
        auto& ball = state->ball;
        const auto& radius = ball->radius;
        ball->velocity += delta * g;
        ball->transform.position += delta * ball->velocity;
        if (ball->transform.position.y <= radius) {
            ball->velocity = -ball->velocity;
            ball->transform.position.y =
                    radius + (radius - ball->transform.position.y);
        }
    }
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
