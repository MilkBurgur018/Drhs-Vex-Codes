#include "main.h"
#include "globals.hpp"
#include "autons.hpp"

void right_qual_auton() {
  //auton init
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    pros::delay(20);
    
    chassis.drive_angle_set(90);

    //Movement
    intake_motor.move_velocity(600); 
    outtake_motor.move_velocity(500);

    chassis.pid_drive_set(42, 77);
    chassis.pid_wait(); 

    chassis.pid_turn_set(0, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(100, 67);
    chassis.pid_wait();

    chassis.pid_drive_set(-100, 67);
    chassis.pid_wait();

    chassis.pid_turn_set(89, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(71, 77);
    chassis.pid_wait();

    chassis.pid_turn_set(180, 90);
    chassis.pid_wait();

    pneumA.set_value(true); 
    pros::delay(500);       

    //ram it in
    chassis.drive_set(80, 80);
    pros::delay(900);
    chassis.drive_set(0, 0);

    //pre sicko
    chassis.pid_turn_set(179, 90);
    chassis.pid_wait();
    chassis.drive_brake_set(MOTOR_BRAKE_COAST);
    pros::delay(20);
    // --- When the robot goes sicko mode ---
    for(int i = 0; i < 13; i++) {
        //using direct voltage calls to shake the robot. bc why the hell not
        chassis.drive_set(12000, 12000);
        pros::delay(50);             
        
        chassis.drive_set(-12000, -12000); 
        pros::delay(50);            
    }   
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    // Stop all drivetrain motors
    left_mg.move_velocity(0);
    right_mg.move_velocity(0);

    // wrap it up
    chassis.pid_drive_set(-80, 77);
    chassis.pid_wait();

    outtake_motor.move_velocity(-12000); 
    pros::delay(4000);
    outtake_motor.move_velocity(0);

    pneumA.set_value(false); 

    //Ram
    chassis.drive_set(40, 40);
    pros::delay(900); 
    chassis.drive_set(0, 0);

    chassis.drive_set(-12000, -12000);
    pros::delay(900);
    chassis.drive_set(0, 0);
}