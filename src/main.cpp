#include <rg/renderer/Scene.hpp>
#include <rg/renderer/light/DirectionalLight.hpp>
#include <rg/util/read_file.hpp>

#include <glm/glm.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const char* TITLE = "Basketball";

int main() {
    rg::Scene* renderer =
            rg::Scene::Builder()
                    .set_window_width(SCR_WIDTH)
                    .set_window_height(SCR_HEIGHT)
                    .set_window_title(TITLE)
                    .set_camera_position(glm::vec3{0.0f, 0.0f, -8.0f})
                    .set_camera_direction(glm::vec3{0.0f, 0.0f, 1.0f})
                    .addModel("ball", "ball/basketball.obj")
                    .addShader("example",
                               rg::ShaderData::Builder()
                                       .addModel("ball")
                                       .set_model_scale(
                                               glm::vec3{0.25f, 0.25f, 0.25f})
                                       .addLight(new rg::DirectionalLight())
                                       .build())
                    .build();
    renderer->loop();
    delete renderer;
    return 0;
}
