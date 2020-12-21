#ifndef APP_OBJECT_HPP
#define APP_OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Transform.hpp>

namespace app {

struct Node {
    rg::Transform transform;
    std::shared_ptr<rg::Model> model;

    Node* parent = nullptr;
    [[nodiscard]] const rg::Transform& get_transform() const;
};

} // namespace app

#endif // APP_OBJECT_HPP
