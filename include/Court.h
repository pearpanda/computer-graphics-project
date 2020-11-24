#ifndef RG_COURT_H
#define RG_COURT_H

/**
 * Class for various configuration properties.
 */
class Court {
public:
    Court();
    Court(float court_width, float hoop_radius);
private:
    float court_width = 15;
    float court_length = 28;
    float hoop_radius = 0.229;
    //there's probably more config to be added here
};
#endif // RG_COURT_H
