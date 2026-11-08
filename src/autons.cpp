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
    lift.move_absolute(240, 127); // move lift up while toggling

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
    chassis.moveToPoint(19.457, -50.282,  750, {.maxSpeed = 127});
    pros::delay(750);

    chassis.setPose(17.227, -50.465, chassis.getPose().theta);

    lift.move_absolute(0, 127); // move the lift back down
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(100);

    chassis.moveToPoint(6.541, -59, 700, {.forwards = false, .maxSpeed = 127}); // back out from goal
    pros::delay(700);

    chassis.turnToPoint(12.3, -60.67, 750, {.minSpeed = 40}); // turn to a point before swinging
    pros::delay(750);

    chassis.moveToPoint(12.3, -60.67, 600, {.minSpeed = 40}); // drive to that point to set up swing for cup
    pros::delay(600);

    chassis.swingToHeading(210, lemlib::DriveSide::RIGHT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(800);

    left_mg.move(100);
    right_mg.move(60);
    pros::delay(200);
    claw.extend();
    pros::delay(50);
    left_mg.move(0);
    right_mg.move(0);

    chassis.setPose(23.481, -61, 180);
    lift.move_absolute(360, 127);

    chassis.turnToHeading(115, 600, {.maxSpeed = 127, .minSpeed = 40});
    pros::delay(600);

    chassis.moveToPoint(6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
    pros::delay(650);

    chassis.moveToPoint(23, -50.5, 600, {.maxSpeed = 127});
    pros::delay(600);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(400);
    claw.retract();
    
    chassis.setPose(16.981, -49.215, chassis.getPose().theta);

    chassis.moveToPoint(2.47, -55.095, 600, {.forwards = false, .maxSpeed = 127, .minSpeed = 30});
    pros::delay(600);
    lift.move_absolute(0, 127);

    chassis.turnToHeading(245.67, 500, {.maxSpeed = 127, .minSpeed = 50});
    pros::delay(500);

    chassis.moveToPoint(-10.2, -59, 700, {.maxSpeed = 127, .minSpeed = 30});
    pros::delay(700);

    chassis.swingToHeading(160, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(800);

    right_mg.move(100);
    left_mg.move(60);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    right_mg.move(0);
    left_mg.move(0);
    
    chassis.setPose(-23.481, -61, 180);

    lift.move_absolute(400, 127);

    chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 40});
    pros::delay(600);

    chassis.moveToPoint(-6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
    pros::delay(650);

    chassis.moveToPoint(-23, -49.8, 600, {.maxSpeed = 127});
    pros::delay(600);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(400);
    claw.retract();

}


//  chassis.setPose(0, -61.515, 0);

    // claw.extend(); // retracts claw to grab hold of the preload
    // lift.move_absolute(240, 127); // move lift up while toggling

    // uint32_t startTime = pros::millis();

    // bool toggleDone = false; // this block flips the toggle to desired color
    // while (!toggleDone) { // (before moving on)
    //     toggleDone = autoToggle(redTeam);
    //     if (pros::millis() - startTime > 1700) {
    //         toggleDone = true;
    //     }
    // }

    // chassis.turnToPoint(19.457, -50.282, 550, {.minSpeed = 30, .earlyExitRange = 10});

    // // drive to alliance goal
    // chassis.moveToPoint(19.457, -50.282,  750, {.maxSpeed = 127});
    // pros::delay(750);

    // chassis.setPose(17.227, -50.465, chassis.getPose().theta);

    // lift.move_absolute(0, 127); // move the lift back down
    // pros::delay(200);

    // claw.retract(); // open the claw to drop the pin
    // pros::delay(100);

    // chassis.moveToPoint(6.541, -59, 700, {.forwards = false, .maxSpeed = 127}); // back out from goal

    // chassis.turnToPoint(13, -60.67, 700, {.maxSpeed = 127, .minSpeed = 40}); // turn to a point before swinging
    // pros::delay(700);

    // chassis.moveToPoint(13, -60.67, 600, {.minSpeed = 40}); // drive to that point to set up swing for cup
    // pros::delay(600);

    // chassis.swingToHeading(210, lemlib::DriveSide::RIGHT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    // pros::delay(800);

    // left_mg.move(100);
    // right_mg.move(60);
    // pros::delay(200);
    // claw.extend();
    // pros::delay(50);
    // left_mg.move(0);
    // right_mg.move(0);

    // chassis.setPose(23.481, -61, 180);
    // lift.move_absolute(360, 127);

    // chassis.turnToHeading(115, 600, {.maxSpeed = 127, .minSpeed = 40, .earlyExitRange = 10});

    // chassis.moveToPoint(6.541, -59, 650, {.forwards = false, .maxSpeed = 127});

    // chassis.moveToPoint(23, -50.5, 600, {.maxSpeed = 127});
    // pros::delay(600);

    // lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    // pros::delay(400);
    // claw.retract();
    
    // chassis.setPose(16.981, -49.215, chassis.getPose().theta);

    // chassis.moveToPoint(2.47, -55.095, 600, {.forwards = false, .maxSpeed = 127, .minSpeed = 40, .earlyExitRange = 3});
    // pros::delay(350);
    // lift.move_absolute(0, 127);

    // chassis.turnToHeading(245.67, 500, {.maxSpeed = 127, .minSpeed = 50});

    // chassis.moveToPoint(-12.3, -59.5, 700, {.maxSpeed = 127, .minSpeed = 30});
    // pros::delay(700);

    // chassis.swingToHeading(160, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    // pros::delay(800);

    // right_mg.move(100);
    // left_mg.move(60);
    // pros::delay(200);
    // claw.extend();
    // pros::delay(250);
    // right_mg.move(0);
    // left_mg.move(0);
    
    // chassis.setPose(-23.481, -61, 180);

    // lift.move_absolute(400, 127);

    // chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 40});
    // pros::delay(600);

    // chassis.moveToPoint(-6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
    // pros::delay(650);

    // chassis.moveToPoint(-23, -49.8, 600, {.maxSpeed = 127});
    // pros::delay(600);

    // lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    // pros::delay(400);
    // claw.retract();





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
