#include "main.h"

// --- INTAKE DEFINITION ---
// Same for all bots (unless you need to change ports, do it here)
pros::Motor intake_motor(7);
pros::Motor outtake_motor(-8);
pros::adi::DigitalOut pneumA('A');
pros::adi::DigitalOut pneumB('B');
//define motors again in pros for auton and more control
pros::MotorGroup left_mg({1, -2, -3});
pros::MotorGroup right_mg({4, 5, 6});

// --- CHASSIS DEFINITIONS ---

// CONFIG 1: COMP BOT (CURRENT) - Green Motors (200), 36:60 Gearing (120 RPM Output)
/*
ez::Drive chassis (
  // Left Motors (ports -1, -2, -3)
  {-1, -2, -3},
  // Right Motors (ports 4, 5, 6)
  {4, 5, 6},
  // IMU Port
  10,
  // Wheel Diameter (3.25" usually for 120rpm setups, change if needed)
  3.25,
  // Cartridge RPM (Green = 200)
  200,
  // External Gear Ratio (36 tooth driving 60 tooth = 0.6 reduction)
  (36.0 / 60.0) 
);
*/


// CONFIG 2: COMP BOT (FUTURE) - Blue Motors (600), 36:60 Gearing

ez::Drive chassis (
  {-1, -2, -3},
  {4, 5, 6},
  20,// IMU Port
  3.25,
  600, // Blue Cartridge
  (24.0 / 60.0) // Keeps the same physical gears
);



// CONFIG 3: TEST BOT - Old Ports
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