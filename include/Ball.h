#ifndef RG_BALL_H
#define RG_BALL_H

class Ball {
public:
    Ball(float x, float y, float z);
    Ball(float x, float y, float z, float radius, float mass);

    float get_x();
    float get_y();
    float get_z();
    float get_radius();

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
     * to the hoop. Positive angle goes to the right. Must be between -pi and pi.
     * @param intensity how hard should the ball be thrown, in Newtons. Must be
     * positive.
     */
    void shoot(float pitch, float yaw, float intensity);

    /**
     * Advance the ball position.
     *
     * This is valid only after the ball has been thrown, otherwise does nothing.
     * Do _not_ assume that because advance() did nothing after one invocation,
     * it won't do anything in the next one: use the hasStopped() method for
     * checking whether the ball has stopped moving.
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
    bool hasStopped();

private:
    //position
    float x, y, z;
    //velocities
    float v_x=0, v_y=0, v_z=0;
    //accelerations
    float a_x=0, a_y=0, a_z=0;

    float mass = 0.0625;
    float radius = 0.12;
};

#endif // RG_BALL_H
