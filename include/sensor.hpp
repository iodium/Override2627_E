#pragma once
#include "pros/distance.hpp"

struct Sensor {
    pros::Distance& sensor;
    double forward;
    double right;
    double angle;
};