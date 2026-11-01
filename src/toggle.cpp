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
    double rgbWanted; // hold desired rgb

    if (wantRed) { // set rgb for desired color 
        
            rgbWanted = 0; // hue value for red
        
      
    }
    else {
       
            rgbWanted = 255; //blue
        
      
    }

    // if (wantRed && colorsAreEqual(355, rgb)) { // if we want red and color we see is red
    //     toggle.move(35);
    //     return false;
    // }
    // else if (wantRed && colorsAreEqual(280, rgb)) { // if we want red and color we see is blue
    //     toggle.move(35);
    //     return false;
    // }
    if (!colorsAreEqual(rgb, rgbWanted)) {  // if color we want
        toggle.move(90);
        return false;
    }
    else {
        toggle.move(0);
        return true;
    }
}