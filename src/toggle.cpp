#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>

bool colorsAreEqual(double c1, double c2) {
    if (c1 > 180) {
        c1 = c1 - 360;
    }
    if (c2 > 180) {
        c2 = c2 - 360;
    }
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
            rgbWanted = 40; // hue value for yellow
    }
    else {
            rgbWanted = 355; // hue value for red
    }

    if (wantRed && colorsAreEqual(355, rgb)) {
        toggle.move(35);
        return false;
    }
    else if (wantRed && colorsAreEqual(240, rgb)) {
        toggle.move(35);
        return false;
    }
    else if (colorsAreEqual(rgb, rgbWanted)) { 
        toggle.move(85);
        return false;
    }
    else {
        toggle.move(0);
        return true;
    }
}