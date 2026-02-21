#include "main.h"

// --- 1. MOTOR & DEVICE SETUP ---
// It is often safer to define these globally or ensure they have gearsets defined.
// Left Side: 3, 5, 13
pros::MotorGroup left_mg({3, 5, 13}, pros::E_MOTOR_GEARSET_18); 

// Right Side: 1, 4, 11 (Reversed)
pros::MotorGroup right_mg({-1, -4, -11}, pros::E_MOTOR_GEARSET_18); 

// Center (H-Train): 2, 12
pros::MotorGroup center_mg({2, -12}, pros::E_MOTOR_GEARSET_18); 

pros::Controller master(pros::E_CONTROLLER_MASTER);

void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

void initialize() {
    pros::lcd::initialize();
    pros::lcd::set_text(1, "H-Drive Active");
    pros::lcd::register_btn1_cb(on_center_button);
    
    // Set all motors to coast by default so the robot doesn't jerk to a stop
    left_mg.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    right_mg.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
    center_mg.set_brake_modes(pros::E_MOTOR_BRAKE_COAST);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
    while (true) {
        // --- 2. GET JOYSTICK VALUES ---
        int fwd = master.get_analog(ANALOG_LEFT_Y);    // Forward/Back
        int turn = master.get_analog(ANALOG_RIGHT_X);  // Turn (Right Stick)
        int strafe = master.get_analog(ANALOG_LEFT_X); // Strafe (Left Stick)

        // --- 3. DEADZONE FILTER ---
        // Prevents "humming" or slow creeping if the joysticks don't center perfectly
        if (abs(fwd) < 10) fwd = 0;
        if (abs(turn) < 10) turn = 0;
        if (abs(strafe) < 10) strafe = 0;

        // --- 4. DRIVE MATH ---
        // If the robot moves backwards when you push forward, swap the signs on right_mg ports above.
        // If the robot spins when you push forward, change right_mg to (fwd + turn).
        int left_out = fwd + turn;
        int right_out = fwd - turn;

        // --- 5. APPLY POWER ---
        // .move() takes a value from -127 to 127
        left_mg.move(left_out);
        right_mg.move(right_out);
        center_mg.move(strafe);

        // --- 6. DEBUG TO SCREEN ---
        // This helps you see if the controller is actually talking to the brain
        pros::lcd::print(4, "Fwd: %d Turn: %d", fwd, turn);
        pros::lcd::print(5, "Strafe: %d", strafe);

        pros::delay(20);
    }
}