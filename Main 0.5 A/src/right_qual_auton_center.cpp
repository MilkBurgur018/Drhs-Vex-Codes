#include "main.h"
#include "globals.hpp"

void right_qual_auton_center() {
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);

    chassis.pid_drive_set(30, 67);
    chassis.pid_wait();
}