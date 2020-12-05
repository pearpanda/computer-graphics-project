#include "rg/model/Court.hpp"

namespace rg {

Court::Court() = default;

Court::Court(float court_width, float court_length, float hoop_radius,
             float hoop_height, float hoop_width, float hoop_offset)
        : court_width_{court_width}, court_length_{court_length},
          hoop_radius_{hoop_radius}, hoop_height_{hoop_height},
          hoop_width_{hoop_width}, hoop_offset_{hoop_offset} {
}

float Court::get_court_width() const {
    return court_width_;
}
float Court::get_court_length() const {
    return court_length_;
}
float Court::get_hoop_radius() const {
    return hoop_radius_;
}
float Court::get_hoop_height() const {
    return hoop_height_;
}
float Court::get_hoop_width() const {
    return hoop_width_;
}
float Court::get_hoop_offset() const {
    return hoop_offset_;
}

}; // namespace rg
