#include "main.h"
#include "auton_select.hpp"
#include "autons.hpp"
#include "globals.hpp"

void initialize() {
    chassis.initialize();
    chassis.opcontrol_drive_activebrake_set(0); 
    create_auton_selector();
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
    chassis.pid_targets_reset(); 
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset(); 
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD); 

    switch (selected_auton) {
        case 1: left_qual_auton();   break;
        case 2: right_qual_auton();  break;
        case 3: skills_auton_for_qual(); break;
        case 4: right_qual_auton_center(); break;
        case 5: left_qual_auton_ram(); break;
        case 6: defensive_mode_auton(); break; 
        case 7: high_speed_mode_auton(); break;
        default: printf("No auton selected!\n"); break;
    }
}

void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    
    // --- TOGGLE STATE VARIABLES ---
    bool intake_active = false; 
    bool outtake_active = false;
    bool piston_state = false; 

    // Set drive to coast for driving
    chassis.drive_brake_set(MOTOR_BRAKE_COAST);

    while (true) {
        // --- SAFETY CHECK ---
        if (is_debug_running) {
            pros::delay(20);
            continue;
        }

        // --- DRIVE CONTROL ---
        chassis.opcontrol_arcade_standard(ez::SPLIT);
        
        // --- INPUT TOGGLES ---
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            intake_active = !intake_active; 
        }
        // FIXED: B Button Toggle
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            outtake_active = !outtake_active; 
        } 
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            piston_state = !piston_state; 
            pneumA.set_value(piston_state); 
        }

        // --- MOTOR LOGIC ---
        int intake_voltage = 0;
        int outtake_voltage = 0;
        bool l2_pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

        // 1. Calculate Intake Logic
        if (intake_active) {
            if (l2_pressed) {
                intake_voltage = -12000; 
            } else {
                intake_voltage = 12000;
            }
            
            // CHAINING: Only run the outtake slowly if the Outtake Toggle is OFF
            if (!outtake_active) {
                outtake_voltage = -6000; 
            }
        }

        // 2. Calculate Outtake Logic
        // If the B toggle is ON, it OVERRIDES the chaining from step 1
        if (outtake_active) {
            if (l2_pressed) {
                outtake_voltage = -12000; 
            } else {
                outtake_voltage = 12000; 
            }
        }

        // 3. Apply Final Voltages
        intake_motor.move_voltage(intake_voltage);
        outtake_motor.move_voltage(outtake_voltage);

        pros::delay(ez::util::DELAY_TIME);                                     
    }
}