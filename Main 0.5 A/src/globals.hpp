#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "main.h"

// Declare the EZ-Template chassis object
extern ez::Drive chassis;

// Declare the intake motor
extern pros::Motor intake_motor;
extern pros::Motor outtake_motor;
extern pros::MotorGroup left_mg;
extern pros::MotorGroup right_mg;

extern pros::adi::DigitalOut pneumA;
extern pros::adi::DigitalOut pneumB;

#endif