#include <app/cleanup.hpp>
#include <app/init.hpp>
#include <app/loop.hpp>

float sensitivity = 0.003f;

int main() {
    app::init();
    app::loop();
    app::cleanup();
    return 0;
}
