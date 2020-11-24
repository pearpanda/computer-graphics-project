#ifndef RG_COURT_H
#define RG_COURT_H

/**
 * Class for various configuration properties related to court. All properties
 * are read-only.
 */
class Court {
// This is a trivial class--it contains no logic, only plain constructors and
// accessors, hence definition and implementation are not split and
// private/public block order is swapped (public block is boilerplate).

private:
    float court_width = 15;
    float court_length = 28;
    float hoop_radius = 0.229;
    //there's probably more config to be added here

public:
    Court() {}
    Court(float court_width, float court_length, float hoop_radius) {
        this->court_width = court_width;
        this->court_length = court_length;
        this->hoop_radius = hoop_radius;
    }

    float get_court_width() {
        return court_width;
    }
    float get_court_length() {
        return court_length;
    }
    float get_hoop_radius() {
        return hoop_radius;
    }
};
#endif // RG_COURT_H
