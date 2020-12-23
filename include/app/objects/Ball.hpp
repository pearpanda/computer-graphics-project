#ifndef APP_OBJECTS_BALL_HPP
#define APP_OBJECTS_BALL_HPP

#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Transform.hpp>

#include <memory>

namespace app {

struct Ball {
    rg::Transform transform;
    std::shared_ptr<rg::Model> model;
    glm::vec3 velocity{0.0f}; // meters per second
    float radius = 0.25f;     // meters
};

} // namespace app

#endif // APP_OBJECTS_BALL_HPP
