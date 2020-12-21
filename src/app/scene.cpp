#include <app/init.hpp>

#include <rg/renderer/camera/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <rg/util/common_meshes.hpp>

#include <stb/stb_image.h>

namespace app {

void initScene() {
    initCameras();
    initShaders();
    initSkybox();
    initModels();
}

void initCameras() {
    // Cameras
    // -------
    auto& cameras = state->camera_subsystem.cameras;
    auto width = static_cast<float>(state->window_width);
    auto height = static_cast<float>(state->window_height);

    cameras[0] = new rg::Camera{glm::vec3{-0.5f, -0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                width / height,
                                0.1f,
                                100.0f};
    cameras[1] = new rg::Camera{glm::vec3{0.5f, -0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                width / height,
                                0.1f,
                                100.0f};
    cameras[2] = new rg::Camera{glm::vec3{0.5f, 0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                width / height,
                                0.1f,
                                100.0f};
    cameras[3] = new rg::Camera{glm::vec3{-0.5f, 0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                width / height,
                                0.1f,
                                100.0f};

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
    std::string path = util::resource("skyboxes/night-real/");
    std::vector<std::string> faces{"xpos.png", "xneg.png", "ypos.png",
                                   "yneg.png", "zpos.png", "zneg.png"};
    state->skybox = new rg::Skybox{path, faces};
}

void initModels() {
    stbi_set_flip_vertically_on_load(true);
    std::string backpack_path = util::resource("objects/backpack/backpack.obj");
    state->backpack = new rg::Model{backpack_path};
}

} // namespace app
