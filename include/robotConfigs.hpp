#pragma once
#include "main.h"
#include "lemlib/api.hpp"

extern pros::Controller controller;
extern pros::MotorGroup left_mg, right_mg;
extern pros::MotorGroup lift;
extern pros::adi::DigitalOut clamp;
extern pros::Imu imu;
extern lemlib::Chassis chassis;