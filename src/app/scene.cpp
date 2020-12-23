#include <app/init.hpp>

#include <app/objects/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <rg/util/common_meshes.hpp>

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
    state->ball = new Ball;
    state->lamp = new Lamp;
    state->floor = new Floor;
}

void initCameras() {
    // Cameras
    // -------
    auto& cameras = state->camera_subsystem.cameras;
    auto width = static_cast<float>(state->window_width);
    auto height = static_cast<float>(state->window_height);

    for (auto& camera : cameras) {
        camera = new Camera{glm::vec3{0.0f, 0.0f, 0.0f},
                            glm::vec3{0.0f, 0.0f, -1.0f},
                            60.0f,
                            width / height,
                            0.01f,
                            50.0f};
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

    // Light shader
    // ------------
    state->light_shader = new rg::Shader{rg::Shader::compile(
            util::readFile(util::resource("shaders/light.vs.glsl")),
            util::readFile(util::resource("shaders/light.fs.glsl")))};

#ifdef ENABLE_DEBUG
    // Debug shader
    // ------------
    state->debug_shader = new rg::Shader{rg::Shader::compile(
            util::readFile(util::resource("shaders/debug.vs.glsl")),
            util::readFile(util::resource("shaders/debug.fs.glsl")))};
#endif // ENABLE_DEBUG
}

void initSkybox() {
    std::string path = util::resource("skyboxes/night-real/");
    std::vector<std::string> faces{"xpos.png", "xneg.png", "ypos.png",
                                   "yneg.png", "zpos.png", "zneg.png"};
    state->skybox = new rg::Skybox{path, faces};
}

void initModels() {
    std::string backpack_path = util::resource("objects/ball/ball.obj");
    state->ball->model = std::make_shared<rg::Model>(backpack_path);

    std::string court_tile_path = util::resource("objects/court-tile/tile.obj");
    state->floor->tile = std::make_shared<rg::Model>(court_tile_path);

    std::string lamp_base_path = util::resource("objects/lamp/base.obj");
    std::string lamp_frame_path = util::resource("objects/lamp/head.obj");
    std::string lamp_light_path = util::resource("objects/lamp/light.obj");
    state->lamp->base = std::make_shared<rg::Model>(lamp_base_path);
    state->lamp->frame = std::make_shared<rg::Model>(lamp_frame_path);
    state->lamp->source = std::make_shared<rg::Model>(lamp_light_path);

#ifdef ENABLE_DEBUG
    std::string cube_path = util::resource("objects/cube/cube.obj");
    state->debug_cube = new rg::Model{cube_path};
#endif // ENABLE_DEBUG
}

void setScene() {
    placeObjects();
    placeCameras();
    placeLights();
}

void placeObjects() {
    state->ball->transform.position = glm::vec3{0.0f, 1.0f, 0.0f};
    state->ball->transform.orientation = glm::quat{glm::vec3{0.0f}};
    state->ball->transform.scale = glm::vec3{1.0f};

    state->floor->transform.position = glm::vec3{0.0f};
    state->floor->transform.orientation = glm::quat{glm::vec3{0.0f}};
    state->floor->transform.scale = glm::vec3{1.0f};
    state->floor->width = 10;
    state->floor->height = 10;

    state->lamp->transform.position = glm::vec3{-5.0f, 0.0f, -5.0f};
    state->lamp->transform.orientation =
            glm::quat{glm::vec3{0.0f, glm::radians(-45.0f), 0.0f}};
    state->lamp->transform.scale = glm::vec3{1.0f};
}

void placeCameras() {
    auto& cameras = state->camera_subsystem.cameras;
    cameras[0]->set_position(glm::vec3{-0.4f, 1.7f, 0.4f});
    cameras[0]->set_direction(glm::vec3{0.4f, -0.7f, -0.4f});

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
    weak_night_light.direction = glm::vec3{-2.0f, -1.0f, 3.0f};
    weak_night_light.color.ambient = glm::vec3{0.1f};
    weak_night_light.color.diffuse = glm::vec3{0.2f};
    weak_night_light.color.specular = glm::vec3{0.2f};
    lights.directional->push_back(weak_night_light);

    //    rg::SpotLight camera_spotlight;
    //    camera_spotlight.position = glm::vec3{0.0f, 5.0f, 5.0f};
    //    camera_spotlight.direction = glm::vec3{0.0f, -1.0f, -1.0f};
    //    camera_spotlight.color.ambient = glm::vec3{0.1f};
    //    camera_spotlight.color.diffuse = glm::vec3{1.0f};
    //    camera_spotlight.color.specular = glm::vec3{1.0f};
    //    camera_spotlight.cutoff_angle = glm::cos(glm::radians(20.0f));
    //    camera_spotlight.weaken_angle = glm::cos(glm::radians(15.0f));
    //    camera_spotlight.attenuation.constant = 1.0f;
    //    camera_spotlight.attenuation.linear = 0.35f;
    //    camera_spotlight.attenuation.quadratic = 0.44f;
    //    lights.spotlight->push_back(camera_spotlight);

    rg::SpotLight lamp_spotlight;
    lamp_spotlight.color.ambient = glm::vec3{0.1f};
    lamp_spotlight.color.diffuse = glm::vec3{1.0f};
    lamp_spotlight.color.specular = glm::vec3{1.0f};
    lamp_spotlight.cutoff_angle = glm::cos(glm::radians(20.0f));
    lamp_spotlight.weaken_angle = glm::cos(glm::radians(15.0f));
    lamp_spotlight.attenuation.constant = 1.0f;
    lamp_spotlight.attenuation.linear = 0.35f;
    lamp_spotlight.attenuation.quadratic = 0.44f;
    lights.spotlight->push_back(lamp_spotlight);
    state->lamp->spotlight = &lights.spotlight->back();

    //    rg::PointLight point_light;
    //    point_light.position = glm::vec3{5.0f, 0.0f, 0.0f};
    //    point_light.color.ambient = glm::vec3{0.1f};
    //    point_light.color.diffuse = glm::vec3{1.0f};
    //    point_light.color.specular = glm::vec3{1.0f};
    //    lights.point->push_back(point_light);
}

} // namespace app
