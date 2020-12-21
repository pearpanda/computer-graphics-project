#include <app/init.hpp>

#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

namespace app {

State* state = nullptr;

void init() {
    state = new State();
    if (state == nullptr) {
        spdlog::error("ERROR::init: Failed to set up program state.");
        throw std::runtime_error{"Program initialization failed"};
    }

    initGraphics();
    bindCallbacks();
    initScene();
}

std::string util::resource(const std::string& path) {
    std::stringstream stream;
    stream << RESOURCE_DIRECTORY << "/" << path;
    return stream.str();
}

std::string util::readFile(const std::string& path) {
    std::fstream file{path};
    std::stringstream medium{};
    medium << file.rdbuf();
    return medium.str();
}

} // namespace app
