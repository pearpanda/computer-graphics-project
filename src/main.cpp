#include <rg/renderer/Renderer.hpp>
#include <rg/util/read_file.hpp>

#include <glm/glm.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const char* TITLE = "Basketball";

int main() {
    rg::Renderer* renderer =
            (new rg::Renderer::Builder())
                    ->set_window_width(SCR_WIDTH)
                    ->set_window_height(SCR_HEIGHT)
                    ->set_window_title(TITLE)
                    ->set_camera_position(glm::vec3{0.0f, 0.0f, -12.0f})
                    ->set_camera_direction(glm::vec3{-0.1f, -0.1f, 0.99f})
                    ->addShader("example")
                    ->addModel("ball/basketball.obj")
                    ->set_model_translate(glm::vec3{0.0f, 10.0f, 10.0f})
                    ->addModelToShader(0, 0)
                    ->build();
    renderer->loop();
    return 0;
}
