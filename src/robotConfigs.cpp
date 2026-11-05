#include "main.h"

#define DIGITAL_SENSOR_PORT_A 'A'
// define piston for claw
pros::ADIPneumatics claw(DIGITAL_SENSOR_PORT_A, false);

// left motor group
pros::MotorGroup left_mg({-17, -16}, pros::MotorGears::blue);
// right motor group
pros::MotorGroup right_mg({20, 19}, pros::MotorGears::blue);

pros::Rotation toggleSensor(5);

// additional motors

pros::MotorGroup lift({-7, 8, 9}, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

// motor for toggle
pros::Motor toggle(11, pros::MotorGears::blue, pros::v5::MotorUnits::degrees);

// define color sensor
pros::v5::Optical colorSensor(12);

// define claw sensor
pros::v5::Distance clawSensor(15);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_mg, // left motor group
                              &right_mg, // right motor group
                              13.5, // 13.5 inch track width
                              lemlib::Omniwheel::NEW_275, // using 2" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

// imu
pros::Imu imu(3);
// horizontal tracking wheel encoder
pros::Rotation horizontal_encoder(4);
// // vertical tracking wheel encoder
// pros::adi::Encoder vertical_encoder('C', 'D', true);
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_2, -1.75);
// dist from center for later

// // vertical tracking wheel
// lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_2, -2.5);

// odometry settings
lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, //&horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              20, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(1.5, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              5.767, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

// initialize function. Runs on program startup
// void initialize() {
//     pros::lcd::initialize(); // initialize brain screen
//     chassis.calibrate(); // calibrate sensors
//     // print position to brain screen
//     pros::Task screen_task([&]() {
//         while (true) {
//             // print robot location to the brain screen
//             pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
//             pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
//             pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
//             // delay to save resources
//             pros::delay(20);
//         }
//     });
// }