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
                    ->set_camera_position(glm::vec3{1.0f, 2.0f, -12.0f})
                    // todo camera direction is ignored for some reason?
                    ->set_camera_direction(glm::vec3{1.0f, 0.5f, 1.0f})
                    ->addShader("example")
                    ->addModel("ball/basketball.obj")
                    ->addModelToShader(0, 0)
                    ->build();
    renderer->loop();
    return 0;
}
