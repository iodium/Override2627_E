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

bool autoToggle(bool wantRed, bool starting) {
    double rgb = colorSensor.get_hue(); // get current rgb its looking at
    double rgbWanted; // hold desired rgb

    if (wantRed) { // set rgb for desired color 
        if (starting) {
            rgbWanted = 40;
        }
        else {
            rgbWanted = 355; // hue value for red
        }
    }
    else {
        if (starting) {
            rgbWanted = 355;
        }
        else{
            rgbWanted = 240;
        }
    }

    if (colorsAreEqual(rgb, rgbWanted)) { 
        toggle.move(0); // if current rgb matches desired one, stop toggle
        return true;
    }
    else {
        toggle.move(95); // if not, keep the toggle moving
        return false;
    }
}