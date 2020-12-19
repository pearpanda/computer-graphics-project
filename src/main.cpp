#include <rg/renderer/Scene.hpp>
#include <rg/renderer/light/DirectionalLight.hpp>

#include <glm/glm.hpp>
#include <rg/renderer/light/SpotLight.hpp>

// All config at one place. These could be set as defaults elsewhere, but
// debugging something would mean jumping through multiple files to set a
// property; this way, it's all in a single block. We use defaults only for
// stuff which is practically "set in stone" (e.g. constant factor for Point and
// Spotlights) and there's no sane reason to tweak it.
// Ideally, this could all be stored in a config file (but that's an overkill,
// for now).
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 800;
constexpr float CAMERA_DISTANCE = 6;
const char* TITLE = "Basketball";

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
using glm::vec3;
const vec3 BALL_SCALE = vec3{0.1f, 0.1f, 0.1f};
const vec3 DIR_LIGHT_DIR = vec3{1.0f, 0.0f, 0.0f};
const vec3 DIR_LIGHT_DIF = vec3{0.25f, 0.25f, 0.25f};
const vec3 SPT_LIGHT_AMB = vec3{0.2f, 0.2f, 0.2f};
const vec3 SPT_LIGHT_POS = vec3{0.f, 0.f, 1.f};
const vec3 SPT_LIGHT_DIR = vec3{0.f, 0.f, -1.f};
const vec3 ONE = vec3{1.f, 1.f, 1.f};
constexpr float SPT_LIGHT_LIN = 0.3;
constexpr float SPT_LIGHT_QUAD = 0.1;
constexpr float BALL_SHININESS = 32.0f;
#pragma clang diagnostic pop

int main() {
    rg::Initializer::oneCamera(SCR_WIDTH, SCR_HEIGHT, CAMERA_DISTANCE)
            .set_window_title(TITLE)
            .addModel("ball", "ball/basketball.obj")
            .addModel("spotlight", "spotlight/spotlight.obj")
            .addShader("example",
                       rg::ShaderData::Builder()
                               .addModel("spotlight")
                               .set_model_translate(SPT_LIGHT_POS)
                               .addModel("ball")
                               .set_model_scale(BALL_SCALE)
                               .set_shininess(BALL_SHININESS)
                               // add a dimmed directional light
                               .addLight(rg::DirectionalLight::Builder(
                                                 DIR_LIGHT_DIR)
                                                 .set_diffuse(DIR_LIGHT_DIF)
                                                 .build())
                               // add a customized spot light
                               .addLight(rg::SpotLight::Builder(SPT_LIGHT_POS,
                                                                SPT_LIGHT_DIR)
                                                 .set_linear(SPT_LIGHT_LIN)
                                                 .set_quadratic(SPT_LIGHT_QUAD)
                                                 .set_ambient(SPT_LIGHT_AMB)
                                                 .build())
                               .build())
            .init();
    // minimal Initializer, which draws only a black window:
    // rg::Initializer::oneCamera(800, 800, 1).init();

    rg::loop();
    rg::cleanup();

    return 0;
}
