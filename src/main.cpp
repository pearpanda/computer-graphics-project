#include <rg/renderer/Renderer.hpp>
#include <rg/util/read_file.hpp>

#include <glm/glm.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* TITLE = "Basketball";

int main() {
    rg::Renderer* renderer =
            (new rg::Renderer::Builder())
                    ->set_window_width(SCR_WIDTH)
                    ->set_window_height(SCR_HEIGHT)
                    ->set_window_title(TITLE)
                    // we're using default camera params, so won't add clutter
                    // here
                    ->addShader("example")
                    ->addModel("/objects/backpack/backpack.obj")
                    ->addModelToShader(0, 0)
                    ->build();
    renderer->loop();
    return 0;
}
