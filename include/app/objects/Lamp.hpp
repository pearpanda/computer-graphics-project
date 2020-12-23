#ifndef APP_OBJECTS_LAMP_HPP
#define APP_OBJECTS_LAMP_HPP

#include <rg/renderer/light/lights.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Transform.hpp>

#include <memory>

namespace app {

struct Lamp {
    rg::Transform transform;
    std::shared_ptr<rg::Model> base;
    std::shared_ptr<rg::Model> frame;
    std::shared_ptr<rg::Model> source;
    rg::SpotLight* spotlight = nullptr;

    void placeLight() const;
    [[nodiscard]] glm::vec3 get_color() const;
};

} // namespace app

#endif // APP_OBJECTS_LAMP_HPP
