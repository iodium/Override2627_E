#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>

bool colorsAreEqual(double c1, double c2) {
    double dc = std::abs(c1 - c2);

    if (dc > 20) {
        return false;
    }
    return true;
}

bool autoToggle(bool wantRed) {
    double rgb = colorSensor.get_hue(); // get current rgb its looking at
    double rgbWanted; // hold desired rgb

    if (wantRed) { // set rgb for desired color 
        rgbWanted = 0; // hue value for red
    }
    else { // if wantRed is false, then the desired color is blue
        rgbWanted = 226; // hue value for blue
    }

    if (colorsAreEqual(rgb, rgbWanted)) { 
        toggle.move(0); // if current rgb matches desired one, stop toggle
        return true;
    }
    else {
        toggle.move(127); // if not, keep the toggle moving
        return false;
    }
}