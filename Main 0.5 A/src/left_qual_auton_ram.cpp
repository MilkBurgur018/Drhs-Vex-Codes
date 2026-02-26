#include "main.h"
#include "globals.hpp"

void left_qual_auton_ram() {
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);

    chassis.pid_turn_set(180, 90);
    chassis.pid_wait();
    pros::delay(1000);
    chassis.pid_turn_set(-180, 90);
    chassis.pid_wait();
    pros::delay(1000);
    chassis.pid_turn_set(90,0);
    chassis.pid_wait();
    pros::delay(1000);
    chassis.pid_turn_set(-90, 90);
    chassis.pid_wait();
    pros::delay(1000);

}