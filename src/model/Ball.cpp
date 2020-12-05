#include "rg/model/Ball.hpp"
#include <cmath>
#include <iostream>

namespace rg {

// private util functions
struct Ball::Impl {
    static float calcPitch(Ball& self) {
        return std::atan2(self.z_,
                          std::sqrt(self.x_ * self.x_ + self.y_ * self.y_));
    }
    static float calcYaw(Ball& self) {
        return std::atan2(self.y_, -self.x_);
    }

    static float calcDrag(Ball& self, float velocity) {
        return (BALL_DRAG_COEFFICIENT * AIR_DENSITY * self.radius_ *
                self.radius_ * M_PIf32 * velocity) /
               self.mass_;
    }
};

Ball::Ball(Court court, float x, float y, float z)
        : court_{court}, x_{x}, y_{y}, z_{z} {
}

Ball::Ball(Court court, float x, float y, float z, float radius, float mass,
           float coefficientOfRestitution)
        : court_{court}, x_{x}, y_{y}, z_{z}, radius_{radius}, mass_{mass},
          coefficient_of_restitution_{coefficientOfRestitution} {
}

void Ball::shoot(float pitch, float yaw, float intensity) {
    float a_total = intensity / this->mass_;
    this->a_x_ = -a_total * std::cos(yaw) * std::cos(pitch);
    this->a_y_ = a_total * std::sin(yaw) * std::cos(pitch);
    this->a_z_ = a_total * std::sin(pitch) - G;
}

void Ball::advance(float dt) {
    v_x_ += a_x_ * dt;
    v_y_ += a_y_ * dt;
    v_z_ += a_z_ * dt;
    x_ += v_x_ * dt;
    y_ += v_y_ * dt;
    z_ += v_z_ * dt;
    a_x_ -= Impl::calcDrag(*this, v_x_);
    a_y_ -= Impl::calcDrag(*this, v_y_);
    a_z_ -= Impl::calcDrag(*this, v_z_);

    unsigned collisionAxis = hasCollided();
    // first is the common case; for (micro?)optimization's sake
    if (collisionAxis == NONE)
        return;
    if (collisionAxis & X) {
        a_x_ = -a_x_;
        v_x_ = -v_x_ * coefficient_of_restitution_;
    }
    if (collisionAxis & Y) {
        a_y_ = -a_y_;
        v_y_ = -v_y_ * coefficient_of_restitution_;
    }
    if (collisionAxis & Z) {
        a_z_ = -(a_z_ + G) - G;
        v_z_ = -v_z_ * coefficient_of_restitution_;
    }
}

bool Ball::hasStopped() const {
    return this->v_x_ == 0 && this->v_y_ == 0 && this->v_z_ == 0 &&
           this->a_x_ == 0 && this->a_y_ == 0 && this->a_z_ == 0;
}

// we narrow some doubles to floats here, but we don't particularly care: float
// is more than precise enough for our needs.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
unsigned Ball::hasCollided() {
    if (x_ - radius_ <= 0)
        return X;
    if (x_ + radius_ >= court_.get_court_length())
        return X;
    if (y_ - radius_ <= 0)
        return Y;
    if (y_ + radius_ >= court_.get_court_width())
        return Y;
    if (z_ - radius_ <= 0)
        return Z;

    // collision with hoop holder: we're being very rough here, approximating
    // the holder as a single line perpendicular to the blackboards
    float x_h = court_.get_hoop_offset(), y_h = court_.get_court_width() / 2,
          z_h = court_.get_hoop_height();
    if ((z_ - radius_ <= z_h || z_ + radius_ >= z_h) &&
        (y_ - radius_ <= y_h || y_ + radius_ >= y_h) && x_ - radius_ <= x_h) {
        if (std::pow(x_ - x_h, 2) + std::pow(y_ - y_h, 2) +
                    std::pow(z_ - z_h, 2) <=
            std::pow(radius_, 2)) {
            return Z;
        }
    }

    // outer and inner radius of the hoop
    float ro_h = court_.get_hoop_radius() + court_.get_hoop_width(),
          ri_h = court_.get_hoop_radius();
    // collision with the hoop
    if ((z_ - radius_ <= z_h || z_ + radius_ >= z_h) &&
        (y_ - radius_ <= y_h + ro_h || y_ + radius_ >= y_h - ri_h) &&
        (x_ - radius_ <= x_h + 2 * ro_h || x_ + radius_ >= x_h)) {
        // radius of the circle which is the intersection of ball and hoop plane
        float r_plane = std::sqrt(std::pow(radius_, 2) - std::pow(z_ - z_h, 2));
        // distance between centers of the ^ circle and hoop circle
        float d_centers_sq = std::pow(x_ - x_h, 2) + std::pow(y_ - y_h, 2);
        bool innerCollision = std::pow(r_plane - ri_h, 2) <= d_centers_sq &&
                              d_centers_sq <= std::pow(r_plane + ri_h, 2);
        bool outerCollision = std::pow(r_plane - ro_h, 2) <= d_centers_sq &&
                              d_centers_sq <= std::pow(r_plane + ro_h, 2);
        int collisions = innerCollision + outerCollision;
        switch (collisions) {
            case 0:
                return NONE;
            case 1:
                // collision with only one circle of the hoop (inner or outer):
                // we're probably hitting it sideways, so just return the ball
                // to where it came from (this does not handle barely touching
                // the hoop properly).
                return X | Y | Z; // NOLINT(hicpp-signed-bitwise)
            case 2:
                if (std::abs(z_ - z_h) < radius_ / 3) {
                    // the collision is almost horizontal, so we'll return the
                    // ball to where it came from (almost); this is not a good
                    // approximation, todo to be tested in practice
                    return X | Y | Z; // NOLINT(hicpp-signed-bitwise)
                } else {
                    return Z;
                }
            default:
                std::cerr << "Detected more than two collisions with the hoop: "
                          << collisions << ". This is impossible!";
                throw std::runtime_error{"Impossible collision happened!"};
        }
    }
    return NONE;
}
#pragma clang diagnostic pop

float Ball::get_x() const {
    return x_;
}
float Ball::get_y() const {
    return y_;
}
float Ball::get_z() const {
    return z_;
}
float Ball::get_radius() const {
    return radius_;
}

} // namespace rg
