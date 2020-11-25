#ifndef RG_COURT_H
#define RG_COURT_H

namespace rg {

/**
 * Class for various configuration properties related to court. All properties
 * are read-only.
 */
class Court {
    // This is a trivial class--it contains no logic, only plain constructors
    // and accessors, hence definition and implementation are not split and
    // private/public block order is swapped (public block is boilerplate).

private:
    float court_width = 15;
    float court_length = 28;
    float hoop_radius = 0.229;
    float hoop_height = 3.05;
    /**
     * Space between the blackboard and hoop.
     */
    float hoop_offset = 0.04;
    // there's probably more config to be added here

public:
    Court() {
    }
    Court(float court_width, float court_length, float hoop_radius,
          float hoop_height, float hoop_offset) {
        this->court_width = court_width;
        this->court_length = court_length;
        this->hoop_radius = hoop_radius;
        this->hoop_height = hoop_height;
        this->hoop_offset = hoop_offset;
    }

    float get_court_width() const {
        return court_width;
    }
    float get_court_length() const {
        return court_length;
    }
    float get_hoop_radius() const {
        return hoop_radius;
    }
    float get_hoop_height() const {
        return hoop_height;
    }
    float get_hoop_offset() const {
        return hoop_offset;
    }
};

} // namespace rg

#endif // RG_COURT_H
