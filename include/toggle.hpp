#pragma once
#include "main.h"
#include "lemlib/api.hpp"

bool autoToggle(bool wantRed, bool starting);
bool colorsAreEqual(pros::c::optical_rgb_s_t c1, pros::c::optical_rgb_s_t c2);