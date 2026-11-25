#pragma once
#include "pros/distance.hpp"

struct Sensor {
    pros::v5::Distance& sensor;
    double forward;
    double right;
    double angle;
};