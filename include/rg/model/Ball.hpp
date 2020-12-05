#ifndef RG_MODEL_BALL_HPP
#define RG_MODEL_BALL_HPP

#include "Court.hpp"

namespace rg {
const float BALL_DRAG_COEFFICIENT = 0.47;
const float AIR_DENSITY = 1.225;
const float G = 9.80665;

class Ball {
public:
    Ball(Court court, float x, float y, float z);
    Ball(Court court, float x, float y, float z, float radius, float mass,
         float coefficientOfRestitution);

    [[nodiscard]] float get_x() const;
    [[nodiscard]] float get_y() const;
    [[nodiscard]] float get_z() const;
    [[nodiscard]] float get_radius() const;

    /**
     * Shoot the ball in a direction.
     *
     * See the image for pitch and yaw:
     * https://upload.wikimedia.org/wikipedia/commons/thumb/c/c1/Yaw_Axis_Corrected.svg/2000px-Yaw_Axis_Corrected.svg.png
     * (roll is irrelevant in our case and is ignored)
     *
     * @param pitch angle between the ground and desired direction. 0 = parallel
     * to the ground. Higher pitch goes higher, negative pitch goes downward.
     * Must be between -pi/2 and pi/2.
     * @param yaw direction angle. 0 = parallel to the side walls, i.e. if ball
     * is located in the middle of the free throw line, 0 yaw will go directly
     * to the hoop. Positive angle goes to the left. Must be between -pi and pi.
     * @param intensity how hard should the ball be thrown, in Newtons. Must be
     * positive.
     */
    void shoot(float pitch, float yaw, float intensity);

    /**
     * Advance the ball position.
     *
     * This is valid only after the ball has been thrown, otherwise does
     * nothing. Do _not_ assume that because advance() did nothing after one
     * invocation, it won't do anything in the next one: use the hasStopped()
     * method for checking whether the ball has stopped moving.
     *
     * @param dt how much time has passed since the last time the ball has
     * advanced, in seconds.
     */
    void advance(float dt);

    /**
     * Check if the ball has stopped moving. If this method returned false, it
     * won't return true again until shoot() has been called.
     * @return true if the ball has stopped moving, and calling advance() will
     * do nothing from now on, false otherwise.
     */
    [[nodiscard]] bool hasStopped() const;

private:
    /**
     * Axis over which to reverse movement. Can be combined using bitwise
     * operations, e.g. X & Y (reflect velocities and accelerations on X and
     * Y axes, but not Z).
     */
    enum CollisionAxis { X = 1u, Y = 2u, Z = 4u, NONE = 0 };
    Court court_;
    // position
    float x_, y_, z_;
    // velocities
    float v_x_ = 0, v_y_ = 0, v_z_ = 0;
    // accelerations
    float a_x_ = 0, a_y_ = 0, a_z_ = 0;

    float mass_ = 0.0625;
    float radius_ = 0.12;
    float coefficient_of_restitution_ = 0.95;

    unsigned hasCollided();

    /**
     * Holds static implementation (private) methods without burdening the
     * header with their declarations. A simplification of PIMPL pattern.
     */
    struct Impl;
};

} // namespace rg

#endif // RG_MODEL_BALL_HPP
