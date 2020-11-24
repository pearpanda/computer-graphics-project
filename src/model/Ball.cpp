//
// Created by luka on 24.11.20..
//

#include "../../include/Ball.h"
#include <cmath>

Ball::Ball(Court court, float x, float y, float z) {
    this->court = court;
    this->x = x;
    this->y = y;
    this->z = z;
}

Ball::Ball(Court court, float x, float y, float z, float radius, float mass) {
    this->court = court;
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
    this->mass = mass;
}

void Ball::shoot(float pitch, float yaw, float intensity) {
    float a_total = intensity / this->mass;
    this->a_x = a_total * std::cos(yaw) * std::cos(pitch);
    this->a_y = a_total * std::sin(yaw) * std::cos(pitch);
    this->a_z = a_total * std::sin(pitch);
}

void Ball::advance(float dt) {
    //todo
}

bool Ball::hasStopped() {
    return this->v_x == 0 && this->v_y == 0 && this->v_z == 0 &&
            this->a_x == 0 && this->a_y == 0 && this->a_z == 0;
}

bool Ball::hasCollided() {
    //todo collision checking logic
    return false;
}


//util functions, to be used from this file only
float calc_pitch(float x, float y, float z) {
    return std::atan2(z, std::sqrt(x*x + y*y))
}
float calc_yaw(float x, float y) {
    return std::atan2(y, x);
}