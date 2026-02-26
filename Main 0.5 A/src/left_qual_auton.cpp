#include "main.h"
#include "globals.hpp"

void left_qual_auton() {
    // --- SETUP ---
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    
    chassis.drive_angle_set(-90);

    // --- MOVEMENT ---
    // Start intake ONLY when moving toward the first ring
    intake_motor.move_velocity(600); 
    outtake_motor.move_velocity(500);

    chassis.pid_drive_set(42, 67);
    chassis.pid_wait(); 

    chassis.pid_turn_set(0, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(100, 43);
    chassis.pid_wait();

    chassis.pid_drive_set(-100, 43);
    chassis.pid_wait();

    chassis.pid_turn_set(-90, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(71, 67);
    chassis.pid_wait();

    /*
    chassis.pid_turn_set(90, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(36, 67);
    chassis.pid_wait();

    outtake_motor.move_velocity(600); 
    pros::delay(1000);
    outtake_motor.move_velocity(0);

    intake_motor.move_velocity(0);

    chassis.pid_drive_set(-36, 67);
    chassis.pid_wait();

    chassis.pid_turn_set(180, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(24, 67); redundant, no need to double into goal, we have storage
    chassis.pid_wait();
    */
    chassis.pid_turn_set(-180, 90);
    chassis.pid_wait();

    pneumA.set_value(true); 
    pros::delay(200);       

    chassis.pid_drive_set(37, 67);
    chassis.pid_wait();

    //pre sicko
    chassis.pid_turn_set(-181, 90);
    chassis.pid_wait();
    chassis.drive_brake_set(MOTOR_BRAKE_COAST);
    pros::delay(20);
    // --- When the robot goes sicko mode ---
    for(int i = 0; i < 3; i++) {
        //using pros native motor calls to shake the robot. bc why the hell not
        chassis.pid_turn_set(-180, 90);
        pros::delay(300);             
        
        chassis.pid_turn_set(-181, 90);
        pros::delay(300);
    }   
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    // Stop all drivetrain motors
    left_mg.move_velocity(0);
    right_mg.move_velocity(0);

    // --- WRAP UP ---
    chassis.pid_drive_set(-100, 67);
    chassis.pid_wait();

    outtake_motor.move_velocity(-600); 
    pros::delay(2000);
    outtake_motor.move_velocity(0);

    pneumA.set_value(false); 
}