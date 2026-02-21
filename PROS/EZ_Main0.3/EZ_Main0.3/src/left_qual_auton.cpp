#include "main.h"
#include "globals.hpp"
/* example
    // Set brake mode
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);

    // EXAMPLE: Move forward 24 inches speed limit 110 (max 0-127)
    chassis.pid_drive_set(24_in, 110);
    chassis.pid_wait(); 

    // EXAMPLE: Turn 90 degrees
    chassis.pid_turn_set(90_deg, 90);
    chassis.pid_wait();

    // Example: Move Backwards slew set
    chassis.slew_drive_set(true);  // Enables global slew
    chassis.slew_drive_constants_set(5_in, 50); // Slew over 5 inches to 50 speed

    // Intake Logic
    intake_motor.move_velocity(200);   
    pros::delay(1000);                 
    intake_motor.move_velocity(0);    
    
    Remember, positive turns are clockwise and negative is counterclockwise or + = left and - = right
    */ 
// This function needs to match the declaration in autons.hpp
void left_qual_auton() {
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    chassis.drive_angle_set(-12_deg);

    intake_motor.move_velocity(200);

    chassis.pid_drive_set(24_in, 110);
    chassis.pid_wait();

    chassis.pid_drive_set(-18_in, 110);
    chassis.pid_wait();

    chassis.pid_turn_set(-78_deg, 90);
    chassis.pid_wait(); 

    chassis.pid_drive_set(36_in, 110);
    chassis.pid_wait();

    chassis.pid_turn_set(90_deg, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(12_in, 110);
    chassis.pid_wait();

    intake_motor.move_velocity(200); //TO:DO - change to outtake device
    pros::delay(1000);// Intake for 1 second
    intake_motor.move_velocity(0);

    chassis.pid_drive_set(-12_in, 110);
    chassis.pid_wait();

    chassis.pid_turn_set(180_deg, 90);
    chassis.pid_wait();

    pneumA.set_value(true); // Apply: true = Extend, false = Retract

    chassis.pid_drive_set(36_in, 110);
    chassis.pid_wait();

    for(int i = 0; i < 3; i++) {
        chassis.pid_drive_set(0.5_in, 110);
        chassis.pid_wait();
        chassis.pid_drive_set(-0.5_in, 110);
        chassis.pid_wait();
    }   

    pros::delay(1000);

    for(int i = 0; i < 3; i++) {
        chassis.pid_drive_set(0.5_in, 110);
        chassis.pid_wait();
        chassis.pid_drive_set(-0.5_in, 110);
        chassis.pid_wait();
    }   

    pros::delay(1000);

    intake_motor.move_velocity(0);

    chassis.pid_drive_set(-24_in, 110);
    chassis.pid_wait();

    pneumA.set_value(false); // Apply: true = Extend, false = Retract

    chassis.pid_turn_set(180_deg, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(36_in, 110);
    chassis.pid_wait();

    //TO:DO - outtake device again.
}
