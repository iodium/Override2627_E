#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>
#include <iostream>
#include <numeric>

std::string_view s = "s";

double getResetDistance(Sensor& s, int wall, double theta) {
    double beam = s.sensor.get() / 25.4;   // mm -> inches
    double w = wall * 90;
    double toRad = M_PI / 180.0;
    double beamPart    = beam * cos((theta + s.angle - w) * toRad);
    double forwardPart = s.forward  * cos((theta - w) * toRad);
    double rightPart   = s.right  * cos((theta + 90 - w) * toRad);
    return beamPart + forwardPart + rightPart;
}

void waitLift(double target, double tolerance = 10, int timeout = 1000) {
    uint32_t start = pros::millis();
    while (pros::millis() - start < timeout) {
        if (std::fabs(lift.get_position() - target) < tolerance) break;
        // after 100 ms to get going, if it's not moving, it's not getting closer
        if (pros::millis() - start > 100 && std::fabs(lift.get_actual_velocity()) < 5) break;
        pros::delay(10);
    }
}

void left4pinFaster(bool redTeam) {
    chassis.setPose(0, -61.515, 0);

    claw.extend(); // retracts claw to grab hold of the preload
    lift.move_absolute(270, 127); // move lift up while toggling

    uint32_t startTime = pros::millis();

    bool toggleDone = false; // this block flips the toggle to desired color
    while (!toggleDone) { // (before moving on)
        toggleDone = autoToggle(redTeam);
        if (pros::millis() - startTime > 2000) {
            toggleDone = true;
        }
    }

    // drive to alliance goal
    chassis.moveToPoint(19.786, -49.161,  1100, {.maxSpeed = 75});
    chassis.waitUntilDone();

    // first pin score (unchanged)
    waitLift(270);
    lift.move_absolute(135, 127);
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);

    // RESET
    double Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    double y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.moveToPoint(14, -55, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    chassis.waitUntilDone();
    lift.move_absolute(0, 127); // lower now instead of after the turn

    // RESET (robot stopped here)
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    // chained: turn hands off to the drive within 10 deg
    chassis.turnToPoint(19, -60, 750, {.minSpeed = 30, .earlyExitRange = 10});
    chassis.moveToPoint(17.8, -60, 1000, {.maxSpeed = 80, .minSpeed = 20}); // swing setup point
    chassis.swingToHeading(200, lemlib::DriveSide::RIGHT, 1000, {.maxSpeed = 127}); // swing to put claw around cup
    chassis.waitUntilDone();
    waitLift(0);

    // cup grab (unchanged)
    left_mg.move(100);
    right_mg.move(40);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    left_mg.move(0);
    right_mg.move(0);

    for (int i = 0; i < 1; i++) {
        double x_offset = rightSensor.sensor.get() / 25.4;
        double x = x_offset - 70;
        chassis.setPose(x, -61, 180);
    }

    lift.move_absolute(480, 127);

    // chained: turn straight into the back-out
    chassis.turnToHeading(115, 600, {.minSpeed = 40, .earlyExitRange = 10});
    chassis.moveToPoint(6.432, -54.763, 650, {.forwards = false, .maxSpeed = 127}); // back out to set up for 2nd score
    chassis.waitUntilDone();

    // RESET (stopped)
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    // chained: turn -> drive
    chassis.turnToPoint(23.505, -47, 500, {.minSpeed = 30, .earlyExitRange = 10});
    chassis.moveToPoint(23.505, -47, 600, {.maxSpeed = 60}); // move to 2nd claw for scoring
    chassis.waitUntilDone();

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    // second pin score (unchanged)
    waitLift(480);
    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(500);
    claw.retract();
    pros::delay(300);

    chassis.moveToPoint(2.47, -50, 700, {.forwards = false, .maxSpeed = 127}); // stops cleanly for the reset
    chassis.waitUntilDone();
    lift.move_absolute(0, 127);

    // RESET (stopped)
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    // chained: turn -> swing setup drive
    chassis.turnToPoint(-16.14, -60.626, 800, {.minSpeed = 30, .earlyExitRange = 10});
    chassis.moveToPoint(-17, -60.5, 800, {.maxSpeed = 127, .minSpeed = 20});
    chassis.swingToHeading(180, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    chassis.waitUntilDone();
    waitLift(0);

    // cup grab (unchanged)
    right_mg.move(60);
    left_mg.move(100);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    right_mg.move(0);
    left_mg.move(0);

    for (int i = 0; i < 1; i++) {
        double x_offset = rightSensor.sensor.get() / 25.4;
        double x = -70 + x_offset;
        chassis.setPose(x, -61, 180);
    }

    lift.move_absolute(300, 127);

    chassis.turnToHeading(245, 600, {.minSpeed = 40, .earlyExitRange = 10});
    chassis.moveToPoint(-6.541, -59, 900, {.forwards = false, .maxSpeed = 65}); // slower to stop the tipping
    chassis.waitUntilDone();

    lift.move_absolute(540, 127);

    chassis.moveToPoint(-23, -49.8, 1200, {.maxSpeed = 100});
    chassis.waitUntilDone();
    waitLift(540);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(400);
    claw.retract();
}


void left4pin(bool redTeam) {
    chassis.setPose(0, -61.515, 0);

    claw.extend(); // retracts claw to grab hold of the preload
    lift.move_absolute(270, 127); // move lift up while toggling

    uint32_t startTime = pros::millis();

    bool toggleDone = false; // this block flips the toggle to desired color
    while (!toggleDone) { // (before moving on)
        toggleDone = autoToggle(redTeam);
        if (pros::millis() - startTime > 2000) {
            toggleDone = true;
        }
    }

    // drive to alliance goal
    chassis.moveToPoint(19.786, -49.161,  1100, {.maxSpeed = 75});
    pros::delay(1100);

    lift.move_absolute(135, 127);
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);

    // RESET
    double Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    double y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);


    chassis.moveToPoint(14, -55, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    pros::delay(750);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.turnToPoint(19, -60, 750, {.maxSpeed = 127}); // turn to a point before swinging
    pros::delay(750);

    lift.move_absolute(0, 127);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.moveToPoint(17.8, -60, 1000, {.maxSpeed = 80, .minSpeed = 10}); // drive to that point to set up swing for cup
    pros::delay(1000);

    chassis.swingToHeading(200, lemlib::DriveSide::RIGHT, 1000, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(1000);

    left_mg.move(100);
    right_mg.move(40);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    left_mg.move(0);
    right_mg.move(0);

    for (int i = 0; i < 1; i++) {
        double x_offset = rightSensor.sensor.get() / 25.4;
        double x = x_offset - 70;
        chassis.setPose(x, -61, 180);
    }

    // chassis.setPose(23.668, -61, 180);

    lift.move_absolute(480, 127);

    chassis.turnToHeading(115, 600, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(600);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.moveToPoint(6.432, -54.763, 650, {.forwards = false, .maxSpeed = 127}); // back out to set up for 2nd score
    pros::delay(650);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.turnToPoint(23.505, -47, 500, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(500);

    chassis.moveToPoint(23.505, -47, 600, {.maxSpeed = 60, .earlyExitRange = 5}); // move to 2nd claw for scoring
    pros::delay(600);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(500);
    claw.retract();
    pros::delay(300);

    chassis.moveToPoint(2.47, -50, 700, {.forwards = false, .maxSpeed = 127, .minSpeed = 10});
    pros::delay(700);
    lift.move_absolute(0, 127);

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.turnToPoint(-16.14, -60.626, 800, {.maxSpeed = 127, .minSpeed = 10});

    chassis.moveToPoint(-16.2, -59.5, 800, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(800);

    chassis.swingToHeading(180, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(800);

    right_mg.move(60);
    left_mg.move(100);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    right_mg.move(0);
    left_mg.move(0);

    for (int i = 0; i < 1; i++) {
        double x_offset = rightSensor.sensor.get() / 25.4;
        double x = -70 + x_offset;
        chassis.setPose(x, -61, 180);
    }

    lift.move_absolute(300, 127);

    chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10, .earlyExitRange = 4});
    pros::delay(600);

    chassis.moveToPoint(-6.541, -59, 700, {.forwards = false, .maxSpeed = 80});
    pros::delay(700);

    lift.move_absolute(540, 127);

    chassis.moveToPoint(-23, -49.8, 700, {.maxSpeed = 100});
    pros::delay(700);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(400);
    claw.retract();






    // chassis.moveToPoint(0, -57.5, 400, {.maxSpeed = 127, .minSpeed = 10});
    // pros::delay(400);
    // chassis.turnToPoint(23.505, -47.161, 600, {.maxSpeed = 127, .minSpeed = 0});
    // pros::delay(600);
    // chassis.moveToPoint(12.54, -53.69,  800, {.maxSpeed = 127});
    //  pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // x
    // pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
	//  pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // x
    // chassis.moveToPoint(0, -61.515,  750, {.forwards = false, .maxSpeed = 127});




    // pros::delay(750);
    //  pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // x
	// pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
	// pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // x
}

void right4pin(bool redTeam) {
    chassis.setPose(0, -61.515, 0);

    claw.extend(); // retracts claw to grab hold of the preload
    lift.move_absolute(240, 127); // move lift up while toggling

    uint32_t startTime = pros::millis();

    bool toggleDone = false; // this block flips the toggle to desired color
    while (!toggleDone) { // (before moving on)
        toggleDone = autoToggle(redTeam);
        if (pros::millis() - startTime > 2000) {
            toggleDone = true;
        }
    }

    // drive to alliance goal
    chassis.moveToPoint(-19.786, -49.161,  1100, {.maxSpeed = 75});
    pros::delay(1100);
    lift.move_absolute(45, 127);
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);

    chassis.moveToPoint(-6.541, -59, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    pros::delay(750);

    chassis.turnToPoint(-15.5, -67, 750, {.maxSpeed = 127, .minSpeed = 10}); // turn to a point before swinging
    pros::delay(750);

    lift.move_absolute(0, 127);

    chassis.moveToPoint(-17, -67, 1000, {.maxSpeed = 80, .minSpeed = 10}); // drive to that point to set up swing for cup
    pros::delay(1000);

    chassis.swingToHeading(160, lemlib::DriveSide::LEFT, 1000, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(1000);

    right_mg.move(100);
    left_mg.move(40);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    left_mg.move(0);
    right_mg.move(0);

    // for (int i = 0; i < 1; i++) {
    //     double x_offset = leftSensor.get() / 25.4;
    //     double x = x_offset - 70;
    //     chassis.setPose(x, -61, 180);
    // }

    chassis.setPose(-23.668, -61, 180);

    lift.move_absolute(360, 127);

    chassis.turnToHeading(245, 1000, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(1000);

    chassis.moveToPoint(-6.432, -54.763, 650, {.forwards = false, .maxSpeed = 127}); // back out to set up for 2nd score
    pros::delay(650);
    // chassis.turnToPoint(23.505, -44, 500, {.maxSpeed = 127, .minSpeed = 10});
    // pros::delay(500);
    chassis.moveToPoint(-18, -49, 600, {.maxSpeed = 100}); // move to 2nd claw scoring
    pros::delay(600);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(200);
    claw.retract();

    // chassis.setPose(17.591, -50.801, chassis.getPose().theta);

    chassis.moveToPoint(-2.47, -51.8, 700, {.forwards = false, .maxSpeed = 127, .minSpeed = 10});
    pros::delay(700);
    lift.move_absolute(0, 127);

    chassis.turnToHeading(115, 600, {.maxSpeed = 127, .minSpeed = 10}); // turn around 180
    pros::delay(600);

    chassis.moveToPoint(19.5, -59, 1000, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(1000);

    chassis.swingToHeading(200, lemlib::DriveSide::RIGHT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(800);

    left_mg.move(60);
    right_mg.move(100);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    right_mg.move(0);
    left_mg.move(0);

    chassis.setPose(23.481, -61, 180);

    lift.move_absolute(540, 127);

    chassis.turnToHeading(115, 600, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(600);

    chassis.moveToPoint(6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
    pros::delay(650);

    chassis.moveToPoint(24, -47.5, 600, {.maxSpeed = 127});
    pros::delay(600);

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(400);
    claw.retract();





    // chassis.moveToPoint(0, -57.5, 400, {.maxSpeed = 127, .minSpeed = 10});
    // pros::delay(400);
    // chassis.turnToPoint(23.505, -47.161, 600, {.maxSpeed = 127, .minSpeed = 0});
    // pros::delay(600);
    // chassis.moveToPoint(12.54, -53.69,  800, {.maxSpeed = 127});
    //  pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // x
    // pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
	//  pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // x
    // chassis.moveToPoint(0, -61.515,  750, {.forwards = false, .maxSpeed = 127});




    // pros::delay(750);
    //  pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // x
	// pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
	// pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // x
}

void JADum(int count){
   // The goal of this is to just do an L-Shape
   /* Psuedocode
    * start: (-180.759, -149.765, 270)
    * Go: (-118.302, -149.765, 270)    -> BACKwards
    * Turn right to heading 0
    * Start cascade up to a specific portion
    * Go: (-118.302, -69.219)          -> Forwards
    * Claw Open
    * Go: (-118.302, -149.765) -> BACKwards
    * Cascade down
    * Turn left to heading 270
    * go: (-180.759, -149.765, 270)    -> Forwards
    */

    // Cascade calculations (derived from notebook) -> 9.75 rotations Is the max height for 6-7(?) pins or 43 inches
    /*
    Pins Scored (Index i)	Target Height (in)	Motor Rotations
    0	                    0.0 in	            0.00
    1	                    7.0 in	            1.59
    2	                    13.5 in	            3.06
    3	                    20.0 in	            4.53
    4	                    26.5 in	            6.01
    5	                    33.0 in	            7.48
    6	                    39.5 in	            8.96
    */

    // Wait aren't there 2 pins already there
    double pinRotations[3] = {4.53, 6.01, 7.48};
    const double centi = 2.54; // Movement is in inches right?

    chassis.setPose(-180/centi, -149.765/centi, 270); // Start
    chassis.moveToPoint(-118.302/centi, -149.765/centi, 1000, {  // Drive horizontally along the X axis
        .forwards = false,
        .maxSpeed = 100,
        .minSpeed = 10
    });
    pros::delay(1000);

    // chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10}); -> This is the reference code
    chassis.turnToHeading(0, 600, {.maxSpeed = 100, .minSpeed = 10});
    pros::delay(600);

    lift.move_absolute(pinRotations[count]*360, 100); // The origional 540/127 was DEGREES right?

    // Drive vertically along the Y axis
    chassis.moveToPoint(-118.302/centi, -69.219/centi, 1000, {
        .forwards = true,
        .maxSpeed = 100,
        .minSpeed = 10
    });
    pros::delay(1000);

    // Open claw to release
    claw.retract();
    pros::delay(200);

    // Pin has been theoretically dropped

    // Lower cascade
    lift.move_absolute(-1*pinRotations[count]*60, 100); // The origional 540/127 was DEGREES right?
    pros::delay(400);

    // Traverse the y axis
    chassis.moveToPoint(-118.302/centi, -149.765/centi, 1000, {
        .forwards = false,
        .maxSpeed = 127
        });
    pros::delay(1000);

    // Turn left to face heading 270
    chassis.turnToHeading(270, 750, {
        .maxSpeed = 100,
        .minSpeed = 10
    });
    pros::delay(750);

    // Go back to the start to load the next pin
    chassis.moveToPoint(-185/centi, -149.765/centi, 400, {
        .forwards = true,
        .maxSpeed = 127
        });
    pros::delay(400);
}

void skills(bool redTeam) {
    chassis.setPose(0, -61.515, 0);

    claw.extend(); // retracts claw to grab hold of the preload
    lift.move_absolute(270, 127); // move lift up while toggling

    uint32_t startTime = pros::millis();

    bool toggleDone = false; // this block flips the toggle to desired color
    while (!toggleDone) { // (before movin)
        toggleDone = autoToggle(redTeam);
        if (pros::millis() - startTime > 2000) {
            toggleDone = true;
        }
    }

    // drive to alliance goal
    chassis.moveToPoint(19.786, -49.161,  1100, {.maxSpeed = 75});
    chassis.waitUntilDone();

    // first pin score (unchanged)
    waitLift(270);
    lift.move_absolute(200, 127);
    pros::delay(500);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);

    // RESET
    double Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    double y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    chassis.moveToPoint(6.824, -53.227, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    chassis.waitUntilDone();
    lift.move_absolute(0, 127); // lower now instead of after the turn

    // RESET
    Dy = getResetDistance(rightSensor, 2, chassis.getPose().theta);
    y = -70 + Dy;
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    
    chassis.turnToPoint(20.647, -29.354, 800, {.maxSpeed = 127, .minSpeed = 50, .earlyExitRange = 4});
    chassis.moveToPoint(20.647, -29.354, 1000, {.maxSpeed = 70});
    
    while (clawSensor.get() > 60) {

    }
    claw.extend(); // grabs second pin/cup
    chassis.waitUntilDone();

    lift.move_absolute(405, 127);
    pros::delay(300);

    chassis.turnToPoint(25, -40.078, 600);
    chassis.moveToPoint(25, -40.078, 800);
    chassis.waitUntilDone();
    
    // RESET
    double Dx = getResetDistance(rightSensor, 3, chassis.getPose().theta);
    double x = Dx - 70;
    // if (x < 28) {
    // chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);
    // }
    
    lift.move_absolute(135, 127);
    pros::delay(300);
    claw.retract(); // score second pin/cup
    pros::delay(100);
    
    chassis.moveToPoint(24.534, -25, 1250, {.forwards = false});
    chassis.waitUntilDone();

    // // RESET
    // Dx = getResetDistance(rightSensor, 3, chassis.getPose().theta);
    // x = Dx - 70;
    // if (20 < x < 28) {
    //     chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);
    // }

    chassis.turnToPoint(43.293, -40.796, 600, {.minSpeed = 50});
    lift.move_absolute(50, 127);
    chassis.moveToPoint(43.293, -40.796, 1000, {.maxSpeed = 60});

    while (clawSensor.get() > 55) {

    }

    claw.extend();
    chassis.waitUntilDone();

    lift.move_absolute(600, 127);
    pros::delay(300);

    chassis.turnToPoint(30.5, -47.655, 600, {.maxSpeed = 127, .minSpeed = 40, .earlyExitRange = 5});
    chassis.moveToPoint(30.5, -47.655, 600, {.maxSpeed = 127});
    chassis.waitUntilDone();

    lift.move_absolute(400, 127);

    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);

    pros::delay(400);
    claw.retract();
    pros::delay(200);

    chassis.moveToPoint(58, -47.655, 1000, {.forwards = false});
    lift.move_absolute(0, 127);
    chassis.waitUntilDone();
    
    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);

    chassis.turnToHeading(180, 600);
    
    chassis.moveToPoint(58, -66.871, 1000); // drive into loader
    chassis.waitUntilDone();
    claw.extend(); // grab match load
    pros::delay(400);

    chassis.setPose(chassis.getPose().x, chassis.getPose().y, 180);
    
    lift.move_absolute(135, 127);
    chassis.moveToPoint(58, -44.655, 800, {.forwards = false}); // back out from loader
    pros::delay(400);
    lift.move_absolute(840, 127); // raise lift enough to score
    
    chassis.turnToPoint(25.178, -43.021, 700); // turn to goal
    pros::delay(100);

    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);
    
    chassis.moveToPoint(25.178, -43.021, 1000, {.maxSpeed = 60}); // move to goal
    pros::delay(1000);
    lift.move_absolute(580, 127);
    claw.retract(); // release pin onto goal

    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);
    
    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);

    pros::delay(400);
    claw.retract();
    pros::delay(200);

    chassis.moveToPoint(58, -47.655, 1000, {.forwards = false});
    lift.move_absolute(0, 127);
    chassis.waitUntilDone();
    
    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);

    chassis.turnToHeading(180, 600);
    
    chassis.moveToPoint(58, -66.871, 1000); // drive into loader
    chassis.waitUntilDone();
    claw.extend(); // grab match load
    chassis.setPose(58.667, -58.667, chassis.getPose().theta);
    pros::delay(400);

    chassis.setPose(chassis.getPose().x, chassis.getPose().y, 180);
    
    lift.move_absolute(135, 127);
    chassis.moveToPoint(58, -44.655, 800, {.forwards = false}); // back out from loader
    pros::delay(400);
    lift.move_absolute(840, 127); // raise lift enough to score
    chassis.turnToPoint(25.178, -43.021, 700); // turn to goal
    chassis.waitUntilDone();
    pros::delay(100);

    // RESET
    Dx = getResetDistance(backSensor, 1, chassis.getPose().theta);
    x = 70 - Dx;
    chassis.setPose(x, chassis.getPose().y, chassis.getPose().theta);
    
    chassis.moveToPoint(25.178, -43.021, 1000, {.maxSpeed = 60}); // move to goal
    pros::delay(1000);
    lift.move_absolute(580, 127);
    claw.retract(); // release pin onto goal

}










// void skills(bool redTeam) {
//     chassis.setPose(0, -61.515, 0);



//     claw.extend(); // retracts claw to grab hold of the preload
//     lift.move_absolute(240, 127); // move lift up while toggling

//     uint32_t startTime = pros::millis();

//     bool toggleDone = false; // this block flips the toggle to desired color
//     while (!toggleDone) { // (before moving on)
//         toggleDone = autoToggle(redTeam);
//         if (pros::millis() - startTime > 2000) {
//             toggleDone = true;
//         }
//     }

//     // drive to alliance goal
//     chassis.moveToPoint(19.786, -49.161,  950, {.maxSpeed = 80});
//     pros::delay(950);
//     lift.move_absolute(45, 127);
//     pros::delay(400);

//     claw.retract(); // open the claw to drop the pin
//     pros::delay(300);

//     //resetY();

//     chassis.moveToPoint(6.541, -59, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
//     pros::delay(750);

//     // resetY();

//     chassis.turnToPoint(16, -63, 750, {.maxSpeed = 127, .minSpeed = 10}); // turn to a point before swinging
//     pros::delay(750);

//     lift.move_absolute(0, 127);

//     chassis.moveToPoint(15.5, -63.5, 1000, {.maxSpeed = 80, .minSpeed = 10}); // drive to that point to set up swing for cup
//     pros::delay(1000);

//     chassis.swingToHeading(200, lemlib::DriveSide::RIGHT, 1000, {.maxSpeed = 127}); // swing to put claw around cup
//     pros::delay(1000);

//     left_mg.move(100);
//     right_mg.move(40);
//     pros::delay(200);
//     claw.extend();
//     pros::delay(250);
//     left_mg.move(0);
//     right_mg.move(0);

//     chassis.setPose(23.668, -61, 180);

//     lift.move_absolute(360, 127);

//     chassis.turnToHeading(115, 1000, {.maxSpeed = 127, .minSpeed = 10});
//     pros::delay(1000);

//     chassis.moveToPoint(6.432, -54.763, 650, {.forwards = false, .maxSpeed = 127}); // back out to set up for 2nd score
//     pros::delay(650);

//     chassis.moveToPoint(17.449, -47, 600, {.maxSpeed = 100}); // move to 2nd claw scoring
//     pros::delay(600);

//     lift.move_absolute(180, 90); // lower lift and open claw to release the cup
//     pros::delay(200);
//     claw.retract();

//     // chassis.setPose(17.591, -50.801, chassis.getPose().theta);

//     chassis.moveToPoint(2.47, -51.8, 700, {.forwards = false, .maxSpeed = 127});
//     pros::delay(700);
//     lift.move_absolute(0, 127);

//     chassis.turnToPoint(8.209, -10.519, 800);
//     pros::delay(800);

//     chassis.moveToPoint(8.209, -10.519, 2000);
//     pros::delay(2000);

//     // chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10}); // turn around 180
//     // pros::delay(600);

//     // chassis.moveToPoint(-9, -57.5, 700, {.maxSpeed = 127, .minSpeed = 10});
//     // pros::delay(700);

//     // chassis.swingToHeading(160, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
//     // pros::delay(800);

//     // right_mg.move(60);
//     // left_mg.move(100);
//     // pros::delay(200);
//     // claw.extend();
//     // pros::delay(250);
//     // right_mg.move(0);
//     // left_mg.move(0);

//     // chassis.setPose(-23.481, -61, 180);

//     // lift.move_absolute(400, 127);

//     // chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10});
//     // pros::delay(600);

//     // chassis.moveToPoint(-6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
//     // pros::delay(650);

//     // lift.move_absolute(180, 90); // lower lift and open claw to release the cup
//     // pros::delay(600);
//     // claw.retract();

//     // chassis.moveToPoint(-12.58, -53.519, 800, {.forwards = false});
//     // pros::delay(800);

// }

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
