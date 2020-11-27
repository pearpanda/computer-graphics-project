#include "rg/model/Ball.hpp"
#include <cmath>

namespace rg {

// private util functions
struct Ball::Impl {
    static float calc_pitch(Ball& self) {
        return std::atan2(self.z, std::sqrt(self.x * self.x + self.y * self.y));
    }
    static float calc_yaw(Ball& self) {
        return std::atan2(self.y, -self.x);
    }

    static float calc_drag(Ball& self, float velocity) {
        return (BALL_DRAG_COEFFICIENT * AIR_DENSITY * self.radius * self.radius *
                M_PIf32 * velocity) / self.mass;
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
    a_x -= Impl::calc_drag(*this, v_x);
    a_y -= Impl::calc_drag(*this, v_y);
    a_z -= Impl::calc_drag(*this, v_z);

    CollisionAxis collisionAxis = hasCollided();
    if(collisionAxis == NONE) return;
    else if(collisionAxis == X) {
        a_x = -a_x;
        v_x = -v_x * coefficientOfRestitution;
    } else if(collisionAxis == Y) {
        a_y = -a_y;
        v_y = -v_y * coefficientOfRestitution;
    } else if(collisionAxis == Z) {
        a_z = -a_z;
        v_z = -v_z * coefficientOfRestitution;
    }
    //todo more complicated collisions (i.e. with hoop)
}

bool Ball::hasStopped() const {
    return this->v_x == 0 && this->v_y == 0 && this->v_z == 0 &&
           this->a_x == 0 && this->a_y == 0 && this->a_z == 0;
}

Ball::CollisionAxis Ball::hasCollided() {
    if(x - radius <= 0) return X;
    if(x + radius >= court.get_court_length()) return X;
    if(y - radius <= 0) return Y;
    if(y + radius >= court.get_court_width()) return Y;
    if(z - radius <= 0) return Z;
    // todo nontrivial collisions
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
