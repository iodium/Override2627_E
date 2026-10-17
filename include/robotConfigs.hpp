#pragma once
#include "main.h"
#include "api.h"
#include "lemlib/api.hpp"

extern pros::Controller controller;
extern pros::MotorGroup left_mg, right_mg;
extern pros::Motor intake, lift;
extern pros::adi::DigitalOut clamp;
extern pros::Imu imu;
extern lemlib::Chassis chassis;