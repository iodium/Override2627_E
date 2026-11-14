#pragma once
#include "main.h"
#include "pros/misc.h"
#include <cmath>
#include <iostream>
#include <numeric>

std::string_view s = "s";


void resetY() {
    double alpha = atan(7.5 / 5) * (180.0 / std::numbers::pi);
    double a = sqrt(std::pow(7.5, 2) + std::pow(5, 2));
    double d = leftSensor.get() / 25.4;
    double theta = 90 - chassis.getPose().theta;

    double diff = a * cos((alpha + theta) * (std::numbers::pi / 180.0));
    double bigPart = d * cos(theta * (std::numbers::pi / 180.0));
    double D = bigPart + diff;
    double y = -70 + D;
    
    chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // heading
	pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
	pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // y
    pros::lcd::print(3, "D: %.5f", D); // D
    pros::lcd::print(4, "theta_mine: %.5f", theta); // x
    pros::lcd::print(5, "diff: %.5f", diff); // x
    pros::lcd::print(6, "d: %.5f", d); // x
    pros::lcd::print(7, "bigPart: %.5f", bigPart); // x
}

void left4pin(bool redTeam) {
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
    chassis.moveToPoint(19.786, -49.161,  950, {.maxSpeed = 80});
    pros::delay(950);
    lift.move_absolute(45, 127);
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);
   
    //resetY();

    chassis.moveToPoint(6.541, -59, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    pros::delay(750);

    // resetY();

    chassis.turnToPoint(16, -63, 750, {.maxSpeed = 127, .minSpeed = 10}); // turn to a point before swinging
    pros::delay(750);

    lift.move_absolute(0, 127);

    chassis.moveToPoint(15.5, -63.5, 1000, {.maxSpeed = 80, .minSpeed = 10}); // drive to that point to set up swing for cup
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
    
    // for (int i = 0; i < 1; i++) {
    //     double x_offset = leftSensor.get() / 25.4;
    //     double x = x_offset - 70;
    //     chassis.setPose(x, -61, 180);
    // }

    chassis.setPose(23.668, -61, 180);
    
    lift.move_absolute(360, 127);

    chassis.turnToHeading(115, 1000, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(1000);

    chassis.moveToPoint(6.432, -54.763, 650, {.forwards = false, .maxSpeed = 127}); // back out to set up for 2nd score
    pros::delay(650);
    // chassis.turnToPoint(23.505, -44, 500, {.maxSpeed = 127, .minSpeed = 10});
    // pros::delay(500);
    chassis.moveToPoint(17.449, -47, 600, {.maxSpeed = 100}); // move to 2nd claw scoring
    pros::delay(600);

    // for (int i = 0; i < 1; i++) {
    //     double alpha = atan(7.5 / 5) * (180.0 / std::numbers::pi);
    //     double a = sqrt(std::pow(7.5, 2) + std::pow(5, 2));
    //     double d = leftSensor.get() / 25.4;
    //     double theta = 90 - chassis.getPose().theta;

    //     double diff = a * cos((alpha + theta) * (std::numbers::pi / 180.0));
    //     double bigPart = d * cos(theta * (std::numbers::pi / 180.0));
    //     double D = bigPart + diff;
    //     double y = -70 + D;
        
    //     chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    //     pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // heading
    //     pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
    //     pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // y
    //     pros::lcd::print(3, "D: %.5f", D); // D
    //     pros::lcd::print(4, "theta_mine: %.5f", theta); // x
    //     pros::lcd::print(5, "diff: %.5f", diff); // x
    //     pros::lcd::print(6, "d: %.5f", d); // x
    //     pros::lcd::print(7, "bigPart: %.5f", bigPart); // x
    // }

    lift.move_absolute(180, 90); // lower lift and open claw to release the cup
    pros::delay(200);
    claw.retract();
    
    // chassis.setPose(17.591, -50.801, chassis.getPose().theta);

    chassis.moveToPoint(2.47, -51.8, 700, {.forwards = false, .maxSpeed = 127, .minSpeed = 10});
    pros::delay(700);
    lift.move_absolute(0, 127);

    chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10}); // turn around 180
    pros::delay(600);

    chassis.moveToPoint(-10, -58, 700, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(700);

    chassis.swingToHeading(160, lemlib::DriveSide::LEFT, 800, {.maxSpeed = 127}); // swing to put claw around cup
    pros::delay(800);

    right_mg.move(60);
    left_mg.move(100);
    pros::delay(200);
    claw.extend();
    pros::delay(250);
    right_mg.move(0);
    left_mg.move(0);
    
    chassis.setPose(-23.481, -61, 180);

    lift.move_absolute(400, 127);

    chassis.turnToHeading(245, 600, {.maxSpeed = 127, .minSpeed = 10});
    pros::delay(600);

    chassis.moveToPoint(-6.541, -59, 650, {.forwards = false, .maxSpeed = 127});
    pros::delay(650);

    chassis.moveToPoint(-23, -49.8, 600, {.maxSpeed = 127});
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
    chassis.moveToPoint(-19.786, -49.161,  950, {.maxSpeed = 80});
    pros::delay(950);
    lift.move_absolute(45, 127);
    pros::delay(400);

    claw.retract(); // open the claw to drop the pin
    pros::delay(300);
   
    //resetY();

    chassis.moveToPoint(-6.541, -59, 750, {.forwards = false, .maxSpeed = 127}); // back out from goal
    pros::delay(750);

    // resetY();

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

    // for (int i = 0; i < 1; i++) {
    //     double alpha = atan(7.5 / 5) * (180.0 / std::numbers::pi);
    //     double a = sqrt(std::pow(7.5, 2) + std::pow(5, 2));
    //     double d = leftSensor.get() / 25.4;
    //     double theta = 90 - chassis.getPose().theta;

    //     double diff = a * cos((alpha + theta) * (std::numbers::pi / 180.0));
    //     double bigPart = d * cos(theta * (std::numbers::pi / 180.0));
    //     double D = bigPart + diff;
    //     double y = -70 + D;
        
    //     chassis.setPose(chassis.getPose().x, y, chassis.getPose().theta);

    //     pros::lcd::print(0, "Theta: %.5f", chassis.getPose().theta); // heading
    //     pros::lcd::print(1, "X: %.5f", chassis.getPose().x); // x
    //     pros::lcd::print(2, "Y: %.5f", chassis.getPose().y); // y
    //     pros::lcd::print(3, "D: %.5f", D); // D
    //     pros::lcd::print(4, "theta_mine: %.5f", theta); // x
    //     pros::lcd::print(5, "diff: %.5f", diff); // x
    //     pros::lcd::print(6, "d: %.5f", d); // x
    //     pros::lcd::print(7, "bigPart: %.5f", bigPart); // x
    // }

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

    lift.move_absolute(520, 127);

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

