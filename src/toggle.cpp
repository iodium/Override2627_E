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

    if (dc > 25) {
        return false;
    }
    return true;
}

bool autoToggle(bool wantRed) {
    double rgb = colorSensor.get_hue(); // get current rgb its looking at

    if (wantRed && colorsAreEqual(rgb, 355)) { // if we want red and color we see is red
        toggle.move(0); // toggle is currently in the desired position
        return true;
    }
    else if (wantRed && colorsAreEqual(rgb, 260)) { // if we want red and color we see is blue
        toggle.move(80); // slow down to prevent overshoot
        return false;
    }
    else if (wantRed){ // if we want red and color the sensor sees is yellow (just keep it spinning)
        toggle.move(90);
        return false;
    }

    if (!wantRed && colorsAreEqual(rgb, 260)) { // if we want blue and color is blue
        toggle.move(0); // toggle is currently in desired position, so stop spinning
        return true;
    }
    else if (!wantRed && colorsAreEqual(rgb, 40)) { // if we want blue & color is yellow
        toggle.move(80); // slow down to prepare
        return false;
    }
    else { // we want blue and color is currently red (just keep it spinning)
        toggle.move(90);
        return false;
    }
    
}