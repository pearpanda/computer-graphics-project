#ifndef APP_OBJECTS_FLOOR_HPP
#define APP_OBJECTS_FLOOR_HPP

#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Transform.hpp>

#include <memory>

namespace app {

struct Floor {
    rg::Transform transform;
    std::shared_ptr<rg::Model> tile;
    int width;
    int height;
};

} // namespace app

#endif // APP_OBJECTS_FLOOR_HPP
