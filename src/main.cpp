#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <bits/stdc++.h>

// Only screen_task may call pros::lcd functions. LLEMU/LVGL has no locking, so
// printing from more than one task (e.g. here and in opcontrol) corrupts the
// screen library and can freeze the program. Other code sets this instead.
const char* currentMode = "initialize";
bool autoToggleOn;
bool wantRedToggle;
long lastToggle = 0;


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	chassis.calibrate(); // calibrate sensors

	lift.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
	toggle.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	colorSensor.set_led_pwm(100); // set led brightness for color sensor

	pros::Task screen_task([]() {
		while (true) {
			// print robot location to the brain screen
			// pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
			// pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
			// pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
			// pros::lcd::print(3, "MODE: %s", currentMode);
			// pros::lcd::print(7, "R1:%d R2:%d", controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1),
			//                  controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2));
			// // delay to save resources
			// pros::delay(20);
		}
	});
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() { currentMode = "disabled"; }

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() { currentMode = "autonomous"; }

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	currentMode = "opcontrol";
	autoToggleOn = false;
	wantRedToggle = true;

	while (true) {
		int dir = controller.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = controller.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		chassis.arcade(dir, turn); //arcade drive

		if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            if (liftTooFar(false)) {
				liftStop();
			}
			else {
				liftAscend(); // lift keeps going up as long as R1 is held
			}        
		}
		else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			if (liftTooFar(true)) {
				liftStop();
			}
			else {
				liftDescend(); // lift keeps going down as long as R2 is held
			} 
		} 
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
			// red toggle
			autoToggleOn = !autoToggleOn;
			wantRedToggle = true;
			if (!autoToggleOn) {
				toggle.move(0);
			}
		}
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			// blue toggle
			autoToggleOn = !autoToggleOn;
			wantRedToggle = false;
			if (!autoToggleOn) {
				toggle.move(0);
			}
		}
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			claw.toggle();
		}
		else {
			liftStop(); // if nothing is held, lift stays stopped
		}

		//continuous macros
		if (autoToggleOn) {
			double x = autoToggle(wantRedToggle);
			if (x==1) {
				if (lastToggle - pros::millis() > 1) {
					autoToggleOn = !autoToggleOn;
					lastToggle = pros::millis();
				}
			}
		}
		// do toggle macro if it's running

		pros::delay(20);
	}
}