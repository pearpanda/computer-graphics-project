#ifndef RG_MODEL_COURT_HPP
#define RG_MODEL_COURT_HPP

namespace rg {

/**
 * Class for various configuration properties related to court. All properties
 * are read-only.
 */
class Court {
    // This is a trivial class--it contains no logic, only plain constructors
    // and accessors

private:
    float court_width = 15;
    float court_length = 28;
    float hoop_radius = 0.229;
    float hoop_height = 3.05;
    float hoop_width = 0.017;
    /**
     * Space between the blackboard and hoop.
     */
    float hoop_offset = 0.04;
    // there's probably more config to be added here

public:
    Court();

    Court(float court_width, float court_length, float hoop_radius,
          float hoop_height, float hoop_width, float hoop_offset);

    [[nodiscard]] float get_court_width() const;
    [[nodiscard]] float get_court_length() const;
    [[nodiscard]] float get_hoop_radius() const;
    [[nodiscard]] float get_hoop_height() const;
    [[nodiscard]] float get_hoop_offset() const;
    [[nodiscard]] float get_hoop_width() const;
};

} // namespace rg

#endif // RG_MODEL_COURT_HPP
