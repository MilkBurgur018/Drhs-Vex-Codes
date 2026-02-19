#include "main.h"
#include "globals.hpp"

void skills_auton_for_qual() {

    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    
    chassis.drive_angle_set(-22);

    intake_motor.move_velocity(600); 

    chassis.pid_drive_set(25, 110);
    chassis.pid_wait(); 

    chassis.pid_drive_set(-16, 110);
    chassis.pid_wait();

    chassis.pid_turn_set(-68, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(36, 110);
    chassis.pid_wait();

    chassis.pid_turn_set(-90, 90);
    chassis.pid_wait();
    

    outtake_motor.move_velocity(600); 
    pros::delay(1000);
    outtake_motor.move_velocity(0);

    intake_motor.move_velocity(0);

    chassis.pid_turn_set(180, 90);
    chassis.pid_wait();

    chassis.pid_drive_set(-24, 110); 
    chassis.pid_wait();
    
    pneumA.set_value(true); 
    pros::delay(200);       

    for(int i = 0; i < 3; i++) {

        left_mg.move_velocity(100);
        right_mg.move_velocity(100);
        pros::delay(600);             
        
        left_mg.move_velocity(-100);
        right_mg.move_velocity(-100);
        pros::delay(600);
    }   

    left_mg.move_velocity(0);
    right_mg.move_velocity(0);

    chassis.pid_drive_set(-24, 110);
    chassis.pid_wait();
    pneumA.set_value(false); 
}