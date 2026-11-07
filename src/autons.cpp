#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>
#include <iostream>
#include <numeric>

std::string_view s = "s";

void gurt(){
    chassis.setPose(64.297, -30.396, 0);
    
    // asdjk;asfjljadsfs
}

void left4pin(bool redTeam) {
    chassis.setPose(0, -61.515, 0);

    claw.extend(); // retracts claw to grab hold of the preload
    lift.move_absolute(360, 127); // move lift up while toggling

    uint32_t startTime = pros::millis();

    bool toggleDone = false; // this block flips the toggle to desired color
    while (!toggleDone) { // (before moving on)
        toggleDone = autoToggle(redTeam);
        if (pros::millis() - startTime > 1500) {
            toggleDone = true;
        }
    }

    chassis.turnToPoint(19.457, -50.282, 600, {.minSpeed = 20});
    pros::delay(550);

    // drive to alliance goal
    chassis.moveToPoint(19.457, -50.282,  1000, {.maxSpeed = 80});
    pros::delay(1000);

    lift.move_absolute(0, 127); // move the lift back down
    pros::delay(500);

    claw.retract(); // open the claw to drop the pin
    pros::delay(500);

    chassis.moveToPoint(8.387, -58.849, 650, {.forwards = false}); // back out from goal
    pros::delay(650);

}



// double og_avg = std::accumulate(og_pos.begin(), og_pos.end(), 0.0) / og_pos.size();
    // uint32_t t1 = pros::millis();
    // while (true) {
    //     std::vector<double> positions = lift.get_position_all();
    //     double avg = std::accumulate(positions.begin(), positions.end(), 0.0) / positions.size();
    //     if (avg < 5 || abs(og_avg - avg) >= 330) {
    //         lift.move(0);
    //         break;
    //     }
    //     if ((pros::millis() - t1 > 550)) {
    //         lift.move(0);
    //         break;
    //     }
    //     else {
    //         lift.move(-127); // lower lift to score the pin into the goal
    //     }
    // }

    // lift.move(0);
