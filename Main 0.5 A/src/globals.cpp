#include "main.h"
#include "globals.hpp"

// Intake / outtake
pros::Motor intake_motor(7);
pros::Motor outtake_motor(-8);

// EZ-Template chassis
ez::Drive chassis (
  {-1, -2, -3},
  {4, 5, 6},
  20,
  3.25,
  600,
  (36.0 / 60.0)
);

// Define MotorGroups
pros::MotorGroup left_mg({-1, -2, 3});
pros::MotorGroup right_mg({4, 5, 6});

// Pneumatics
pros::adi::DigitalOut pneumA('A');
pros::adi::DigitalOut pneumB('B');
pros::adi::DigitalOut pneumH('H');