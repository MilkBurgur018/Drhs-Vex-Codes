#include "main.h"

// --- INTAKE DEFINITION ---
// Same for all bots (unless you need to change ports, do it here)
pros::Motor intake_motor(7);
pros::Motor outtake_motor(-8);
pros::adi::DigitalOut pneumA('A');
pros::adi::DigitalOut pneumB('B');
pros::adi::DigitalOut pneumH('H');
// --- CHASSIS DEFINITIONS ---

// CONFIG 1: COMP BOT - Blue Motors (600), 36:60 Gearing

ez::Drive chassis (
  {-1, -2, -3},
  {4, 5, 6},
  20,// IMU Port
  3.25,
  600, // Blue Cartridge
  (36.0 / 60.0) // Keeps the same physical gears
);



// CONFIG 2: TEST BOT - Old Ports
/*
ez::Drive chassis (
  // Left: -1, -4, -11
  {-1, -4, -11},
  // Right: 3, 5, 13
  {3, 5, 13},
  // IMU Port (Change if needed)
  21,
  // Wheel Diameter
  3.25,
  // Cartridge RPM (Assuming Blue/600 for test bot, change if Green)
  600,
  // Gear Ratio (Direct drive? Change if needed)
  1.0
);
*/

//define motors again in pros for auton and more control
pros::MotorGroup left_mg({1, -2, -3});
pros::MotorGroup right_mg({4, 5, 6});