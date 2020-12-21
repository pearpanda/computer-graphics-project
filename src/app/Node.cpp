#include <app/Node.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace app {

const rg::Transform& Node::get_transform() const {
    return transform;
}

} // namespace app
