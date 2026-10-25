#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>

bool colorsAreEqual(pros::c::optical_rgb_s_t c1, pros::c::optical_rgb_s_t c2) {
    double dr = std::abs(c1.red - c2.red);
    double dg = std::abs(c1.green - c2.green);
    double db = std::abs(c1.blue - c2.blue);
    
    if (dr > 10 || dg > 10 || db > 10) {
        return false;
    }
    return true;
}

void autoToggle(bool wantRed) {
    pros::c::optical_rgb_s_t rgb = colorSensor.get_rgb(); // get current rgb its looking at
    pros::c::optical_rgb_s_t rgbWanted; // hold desired rgb

    if (wantRed) { // set rgb for desired color 
        rgbWanted = pros::c::optical_rgb_s_t{.red = 210, .green = 38, .blue = 48};
    }
    else {
        rgbWanted = pros::c::optical_rgb_s_t{.red = 0, .green = 119, .blue = 200};
    }

    if (colorsAreEqual(rgb, rgbWanted)) { 
        toggle.move(0); // if current rgb matches desired one, stop toggle
    }
    else {
        toggle.move(127); // if not, keep the toggle moving
    }
}