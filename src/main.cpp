#include <rg/renderer/Scene.hpp>
#include <rg/renderer/light/DirectionalLight.hpp>
#include <rg/util/read_file.hpp>

#include <glm/glm.hpp>
#include <rg/renderer/light/SpotLight.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const char* TITLE = "Basketball";

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
using glm::vec3;
const vec3 CAMERA_POS = vec3{0.0f, 0.0f, -8.0f};
const vec3 CAMERA_DIR = vec3{0.0f, 0.0f, 1.0f};
const vec3 MODEL_SCALE = vec3{0.25f, 0.25f, 0.25f};
const vec3 DIR_LIGHT_DIR = vec3{1.0f, 0.0f, 0.0f};
const vec3 DIR_LIGHT_AMB = vec3{0.2f, 0.2f, 0.2f};
const vec3 DIR_LIGHT_DIF = vec3{0.5f, 0.5f, 0.5f};
const vec3 PNT_LIGHT_POS = vec3{3.5f, 3.5f, 3.5f};
const vec3 PNT_LIGHT_AMB = glm::vec3{0.1f, 0.1f, 0.1f};
const vec3 PNT_LIGHT_DIF = glm::vec3{0.6f, 0.6f, 0.6f};
const vec3 PNT_LIGHT_SPE = glm::vec3{1.0f, 1.0f, 1.0f};
const float PNT_LIGHT_LIN = 0.09;
const float PNT_LIGHT_QUAD = 0.032;
const vec3 ZERO = vec3{0.0f, 0.0f, 0.0f};
const float BALL_SHININESS = 32.0f;
#pragma clang diagnostic pop

int main() {
    rg::Scene* renderer =
            rg::Scene::Builder()
                    .set_window_width(SCR_WIDTH)
                    .set_window_height(SCR_HEIGHT)
                    .set_window_title(TITLE)
                    .set_camera_position(CAMERA_POS)
                    .set_camera_direction(CAMERA_DIR)
                    .addModel("ball", "ball/basketball.obj")
                    .addShader("example",
                            rg::ShaderData::Builder()
                                    .addModel("ball")
                                    .set_model_scale(MODEL_SCALE)
                                    .set_shininess(BALL_SHININESS)
                                    .addLight(
                                            rg::PointLight::Builder(
                                                    PNT_LIGHT_POS)
                                                    .set_linear(PNT_LIGHT_LIN)
                                                    .set_quadratic(
                                                            PNT_LIGHT_QUAD)
                                                    .set_ambient(PNT_LIGHT_AMB)
                                                    .set_diffuse(PNT_LIGHT_DIF)
                                                    .set_specular(PNT_LIGHT_SPE)
                                                    .build())
                                    .addLight(
                                            rg::DirectionalLight::Builder(
                                                    DIR_LIGHT_DIR)
                                                    .set_ambient(DIR_LIGHT_AMB)
                                                    .set_diffuse(DIR_LIGHT_DIF)
                                                    .build())
                                    .build())
                    .build();

    renderer->loop();
    delete renderer;
    return 0;
}
