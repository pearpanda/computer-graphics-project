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
const vec3 DIR_LIGHT_DIR = vec3{-0.2f, -1.0f, -0.3f};
const vec3 SPT_LIGHT_POS = vec3{0.0f, 0.0f, 8.0f};
const vec3 SPT_LIGHT_DIR = vec3{0.2f, 1.0f, 0.3f};
const float BALL_SHININESS = 0.5f;
#pragma clang diagnostic pop

// todo figure out why ball is shining like a Christmas tree
//(it's definitely an issue with light implementation, don't know where I
//screwed up)

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
                                       .addLight(rg::DirectionalLight::Builder(
                                                         DIR_LIGHT_DIR)
                                                         .build())
                                       .addLight(rg::SpotLight::Builder(
                                                         SPT_LIGHT_POS,
                                                         SPT_LIGHT_DIR)
                                                         .build())
                                       .build())
                    .build();

    renderer->loop();
    delete renderer;
    return 0;
}
