#include "rg/model/Ball.hpp"
#include <cmath>

namespace rg {

// private util functions
struct Ball::Impl {
    static float calcPitch(Ball& self) {
        return std::atan2(self.z, std::sqrt(self.x * self.x + self.y * self.y));
    }
    static float calcYaw(Ball& self) {
        return std::atan2(self.y, -self.x);
    }

    static float calcDrag(Ball& self, float velocity) {
        return (BALL_DRAG_COEFFICIENT * AIR_DENSITY * self.radius *
                self.radius * M_PIf32 * velocity) /
               self.mass;
    }
};

Ball::Ball(Court court, float x, float y, float z)
        : court{court}, x{x}, y{y}, z{z} {
}

Ball::Ball(Court court, float x, float y, float z, float radius, float mass,
           float coefficientOfRestitution)
        : court{court}, x{x}, y{y}, z{z}, radius{radius}, mass{mass},
          coefficientOfRestitution{coefficientOfRestitution} {
}

void Ball::shoot(float pitch, float yaw, float intensity) {
    float a_total = intensity / this->mass;
    this->a_x = -a_total * std::cos(yaw) * std::cos(pitch);
    this->a_y = a_total * std::sin(yaw) * std::cos(pitch);
    this->a_z = a_total * std::sin(pitch) - G;
}

void Ball::advance(float dt) {
    v_x += a_x * dt;
    v_y += a_y * dt;
    v_z += a_z * dt;
    x += v_x * dt;
    y += v_y * dt;
    z += v_z * dt;
    a_x -= Impl::calcDrag(*this, v_x);
    a_y -= Impl::calcDrag(*this, v_y);
    a_z -= Impl::calcDrag(*this, v_z);

    CollisionAxis collisionAxis = hasCollided();
    if (collisionAxis == NONE)
        return;
    else if (collisionAxis == X) {
        a_x = -a_x;
        v_x = -v_x * coefficientOfRestitution;
    } else if (collisionAxis == Y) {
        a_y = -a_y;
        v_y = -v_y * coefficientOfRestitution;
    } else if (collisionAxis == Z) {
        a_z = -(a_z + G) - G;
        v_z = -v_z * coefficientOfRestitution;
    }
}

bool Ball::hasStopped() const {
    return this->v_x == 0 && this->v_y == 0 && this->v_z == 0 &&
           this->a_x == 0 && this->a_y == 0 && this->a_z == 0;
}

Ball::CollisionAxis Ball::hasCollided() {
    if (x - radius <= 0)
        return X;
    if (x + radius >= court.get_court_length())
        return X;
    if (y - radius <= 0)
        return Y;
    if (y + radius >= court.get_court_width())
        return Y;
    if (z - radius <= 0)
        return Z;

    //collision with hoop holder: we're being very rough here, approximating
    //the holder as a single line perpendicular to the blackboards
    float x_h = court.get_hoop_offset(), y_h = court.get_court_width() / 2,
          z_h = court.get_hoop_height();
    if (z - radius <= z_h && z + radius >= z_h && y - radius <= y_h &&
        y + radius >= y_h && x - radius <= x_h) {
        if (powf(x - x_h, 2) + powf(y - y_h, 2) + powf(z - z_h, 2) <=
            powf(radius, 2)) {
            return Z;
        }
    }

    // todo hoop collision
    return NONE;
}

float Ball::get_x() const {
    return x;
}
float Ball::get_y() const {
    return y;
}
float Ball::get_z() const {
    return z;
}
float Ball::get_radius() const {
    return radius;
}

} // namespace rg
