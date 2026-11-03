#include "main.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <bits/stdc++.h>
#include <iostream>

// Only screen_task may call pros::lcd functions. LLEMU/LVGL has no locking, so
// printing from more than one task (e.g. here and in opcontrol) corrupts the
// screen library and can freeze the program. Other code sets this instead.
const char* currentMode = "initialize";
bool autoToggleOn;
bool wantRedToggle;
bool autoClawOn;
long lastToggle = 0;

/**
 * @brief Exponential function to scale output from controller joysticks
 * @brief Helper function for throttleCurve() and steerCurve()
 * @param input Input value from controller analog stick (out of 127)
 * @param maxOut Maximum output out of 127 (generally equals max input)
 * @param base base = b, where f(x) = a * b^x; changes how "curved" the function is
 * @param base Should generally be between 1-1.1 (requires tuning based on driver preference)
 * @param deadzone Stick's deadzone out of 127 (where output will be 0)
 */
double expoCurve(float input, float maxOut, float base, float deadzone) {
    double a = maxOut / pow(base, maxOut);
    float output;

    if (fabs(input) < deadzone) {
        return 0;
    }
    if (input < 0) {
        return round(-a * pow(base, -input));
    }
    else {
        return round(a * pow(base, input));
    }
}

/**
 * @brief Exponential function to scale output from controller joysticks for throttle
 * @details Uses expoCurve() as a helper function
 * @param input Input value from controller analog stick (out of 127)
 * @param maxOut Maximum output out of 127 (generally equals max input)
 * @param base base = b, where f(x) = a * b^x; changes how "curved" the function is
 * @param base Should generally be between 1-1.1 (tune by preference)
 * @param base Be careful with the base, as output values under .5 will become 0
 * @param deadzone Stick's deadzone out of 127 (where output will be 0)
 */
double throttleCurve(float input, float maxOut, float base, float deadzone) {
    return expoCurve(input, maxOut, base, deadzone);
}

/**
 * @brief Exponential function to scale output from controller joysticks for steering
 * @details Uses expoCurve() as a helper function
 * @param input Input value from controller analog stick (out of 127)
 * @param maxOut Maximum output out of 127 (generally equals max input)
 * @param base base = b, where f(x) = a * b^x; changes how "curved" the function is
 * @param base Should generally be between 1.02-1.1 (tune by preference)
 * @param deadzone Stick's deadzone out of 127 (where output will be 0)
 */
double steerCurve(float input, float maxOut, float base, float deadzone) {
    return expoCurve(input, maxOut, base, deadzone);
}

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

	// pros::Task screen_task([]() {
	// 	while (true) {
	// 		// print robot location to the brain screen
	// 		pros::lcd::print(0, "X: %.5f", chassis.getPose().x); // x
	// 		pros::lcd::print(1, "Y: %.5f", chassis.getPose().y); // y
	// 		pros::lcd::print(2, "Theta: %.5f", chassis.getPose().theta); // heading
	// 		pros::lcd::print(4, "raw: %d", toggleSensor.get_angle() / 100);
	// 		// pros::lcd::print(3, "MODE: %s", currentMode);
	// 		// pros::lcd::print(7, "R1:%d R2:%d", controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1),
	// 		//                  controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2));
	// 		// // delay to save resources
	// 		pros::delay(20);
	// 	}
	// });
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
	autoClawOn = true;

	while (true) {
		// vvv code for unscaled controller input vvv 
		// int dir = controller.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		// int turn = controller.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick

		double dir = throttleCurve(controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), 127.0, 1.04, 10);
		double turn = steerCurve(controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X), 127.0, 1.04, 10);

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
		}
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			// blue toggle
			autoToggleOn = !autoToggleOn;
			wantRedToggle = false;
		}
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
			claw.toggle();
			autoClawOn = false;
		}
		else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			autoClawOn = true;
		}	
		else {
			liftStop(); // if nothing is held, lift stays stopped
		}

		//continuous macros
		if (autoToggleOn) {
			bool foundRightColor = autoToggle(wantRedToggle);
			if (foundRightColor) {
				autoToggleOn = false;
			}
		}

		if (autoClawOn) { // if claw macro is primed
			if (autoClaw()) { // if the macro actually closes the claw
				autoClawOn = false; // unprime the macro
			}
			// else, keep it primed so it's ready to close 
			// when the sensor sees a pin close enough
		}

		


		
		pros::delay(2);
	}
}