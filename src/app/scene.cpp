#include <app/init.hpp>

#include <rg/renderer/camera/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <rg/util/common_meshes.hpp>

#include <stb/stb_image.h>

namespace app {

void initScene() {
    // Load components
    // ---------------
    initObjects();
    initCameras();
    initShaders();
    initSkybox();
    initModels();
}

void initObjects() {
    state->backpack = new Node;
}

void initCameras() {
    // Cameras
    // -------
    auto& cameras = state->camera_subsystem.cameras;
    auto width = static_cast<float>(state->window_width);
    auto height = static_cast<float>(state->window_height);

    for (auto& camera : cameras) {
        camera = new rg::Camera{glm::vec3{0.0f, 0.0f, 0.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                width / height,
                                0.1f,
                                100.0f};
    }

    // Surfaces
    // --------
    auto& surfaces = state->camera_subsystem.surfaces;
    auto surface_quad = rg::util::surfaceQuad();
    for (unsigned int i = 0; i < 4; ++i)
        surfaces[i] = new rg::Surface{state->window_width, state->window_height,
                                      surface_quad};
}

void initShaders() {
    // Default shader
    // --------------
    state->shader = new rg::Shader{rg::Shader::compile(
            util::readFile(util::resource("shaders/shader.vs.glsl")),
            util::readFile(util::resource("shaders/shader.fs.glsl")))};

    // Surface shader
    // --------------
    state->surface_shader = new rg::Shader{rg::Shader::compile(
            util::readFile(util::resource("shaders/surface.vs.glsl")),
            util::readFile(util::resource("shaders/surface.fs.glsl")))};

    // Skybox shader
    // -------------
    state->skybox_shader = new rg::Shader{rg::Shader::compile(
            util::readFile(util::resource("shaders/skybox.vs.glsl")),
            util::readFile(util::resource("shaders/skybox.fs.glsl")))};
}

void initSkybox() {
    std::string path = util::resource("skyboxes/arctic/");
    std::vector<std::string> faces{"right.jpg",  "left.jpg",  "top.jpg",
                                   "bottom.jpg", "front.jpg", "back.jpg"};
    state->skybox = new rg::Skybox{path, faces};
}

void initModels() {
    stbi_set_flip_vertically_on_load(true);
    std::string backpack_path = util::resource("objects/backpack/backpack.obj");
    state->backpack->model = std::make_shared<rg::Model>(backpack_path);
}

void setScene() {
    placeObjects();
    placeCameras();
    placeLights();
}

void placeObjects() {
    state->backpack->transform.position = glm::vec3{0.0f};
    state->backpack->transform.orientation = glm::quat{glm::vec3{0.0f}};
    state->backpack->transform.scale = glm::vec3{0.3f};
}

void placeCameras() {
    auto& cameras = state->camera_subsystem.cameras;
    cameras[0]->set_position(glm::vec3{-0.5f, -0.5f, 3.0f});
    cameras[1]->set_position(glm::vec3{0.5f, -0.5f, 3.0f});
    cameras[2]->set_position(glm::vec3{0.5f, 0.5f, 3.0f});
    cameras[3]->set_position(glm::vec3{-0.5f, 0.5f, 3.0f});
}

void placeLights() {
    // Set up storage
    // --------------
    auto& lights = state->light_subsystem;
    lights.directional = new std::vector<rg::DirectionalLight>();
    lights.point = new std::vector<rg::PointLight>();
    lights.spotlight = new std::vector<rg::SpotLight>();

    rg::DirectionalLight weak_night_light;
    weak_night_light.direction = glm::vec3{-2.0f, -1.0f, 1.0f};
    weak_night_light.color.ambient = glm::vec3{0.1f};
    weak_night_light.color.diffuse = glm::vec3{0.2f};
    weak_night_light.color.specular = glm::vec3{0.1f};
    lights.directional->push_back(weak_night_light);

    rg::SpotLight spotlight;
    spotlight.position = glm::vec3{0.0f, 5.0f, 5.0f};
    spotlight.direction = glm::vec3{0.0f, -1.0f, -1.0f};
    spotlight.color.ambient = glm::vec3{0.1f};
    spotlight.color.diffuse = glm::vec3{1.0f};
    spotlight.color.specular = glm::vec3{1.0f};
    lights.spotlight->push_back(spotlight);

    rg::PointLight point_light;
    spotlight.position = glm::vec3{5.0f, 0.0f, 0.0f};
    spotlight.color.ambient = glm::vec3{0.1f};
    spotlight.color.diffuse = glm::vec3{1.0f};
    spotlight.color.specular = glm::vec3{1.0f};
    lights.point->push_back(point_light);
}

} // namespace app