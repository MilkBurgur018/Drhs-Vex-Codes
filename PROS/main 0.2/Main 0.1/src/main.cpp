#include "main.h"
#include "auton_select.hpp"
#include "autons.hpp"
#include "globals.hpp"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 */
void initialize() {
    create_auton_selector();
}

/**
 * Runs while the robot is in the disabled state.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the FMS.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code.
 */
void autonomous() {
    // This looks at the selected_auton var (1-8) and runs the matching function
    switch (selected_auton) {
        case 1: left_qual_auton();   break;
        case 2: right_qual_auton();  break;
        case 3: skills_auton_for_qual(); break;
        case 4: right_qual_auton_center(); break;
        case 5: left_qual_auton_ram(); break;
        
        default: 
            // Fallback if something goes wrong
            printf("No auton selected!\n");
            break;
    }
}

/**
 * Runs the operator control code.
 */
void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    bool intake_active = false; 

    while (true) {
        
        // --- SAFETY CHECK ---
        // If the debug run is active, SKIP all joystick/button controls.
        // This ensures holding L2 during a run does NOT stop or reverse motors.
        if (is_debug_running) {
            pros::delay(20); // Wait and let the auton task work
            continue;        // Skip the rest of the loop immediately
        }

        // --- INTAKE LOGIC ---
        // Toggle spinning on/off with Y
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            intake_active = !intake_active; 
        }

        int intake_voltage = 0;

        // Only run logic if the intake is supposed to be active
        if (intake_active) {
            // If L2 is held, reverse the motor (-12000)
            // Otherwise, keep it moving forward (12000)
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
                intake_voltage = -12000;
            } else {
                intake_voltage = 12000;
            }
        } else {
            // If intake is not active, voltage is 0
            intake_voltage = 0;
        }

        // Apply the voltage
        intake_motor.move_voltage(intake_voltage); 


        // --- DRIVE LOGIC ---
        int dir = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);    
        int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);  
        left_mg.move(dir + turn);                                      
        right_mg.move(dir - turn);                     
        
        pros::delay(20);                                      
    }
}