#include "rg/model/Court.hpp"

namespace rg {

Court::Court() = default;

Court::Court(float court_width, float court_length, float hoop_radius,
             float hoop_height, float hoop_offset)
        : court_width{court_width}, court_length{court_length},
          hoop_radius{hoop_radius}, hoop_height{hoop_height},
          hoop_offset{hoop_offset} {
}

float Court::get_court_width() const {
    return court_width;
}
float Court::get_court_length() const {
    return court_length;
}
float Court::get_hoop_radius() const {
    return hoop_radius;
}
float Court::get_hoop_height() const {
    return hoop_height;
}
float Court::get_hoop_offset() const {
    return hoop_offset;
}

}; // namespace rg
