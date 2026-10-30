#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>

void autoClaw() {
    int distance = clawSensor.get_distance(); // get reading from claw sensor
    // (measured in mm)
    if (distance < 25.4) { // if reading is under 1 inch (25.4 mm)
        claw.retract(); // automatically close the claw since a pin is close
    }
}