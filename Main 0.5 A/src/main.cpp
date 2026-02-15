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
    
    // --- TOGGLES ---
    bool intake_active = false; 
    bool outtake_active = false;
    bool piston_stateA = false; 
    bool piston_stateB = false;

    // Set drive to coast for driving
    chassis.drive_brake_set(MOTOR_BRAKE_COAST);

    while (true) {
        if (is_debug_running) { pros::delay(20); continue; }

        // --- DRIVE ---
        chassis.opcontrol_arcade_standard(ez::SPLIT);
        
        // --- INPUTS ---
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            intake_active = !intake_active; 
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            outtake_active = !outtake_active; 
        } 
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            piston_stateA = !piston_stateA; 
            pneumA.set_value(piston_stateA); 
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            piston_stateB = !piston_stateB; 
            pneumB.set_value(piston_stateB); 
        }

        // --- MOTOR LOGIC ---
        int intake_voltage = 0;
        int outtake_voltage = 0;
        bool l2_pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);

        // 1. INTAKE LOGIC
        if (intake_active) {
            if (l2_pressed) {
                intake_voltage = -12000; // Reverse
            } else {
                intake_voltage = 12000;  // Forward
            }
        } else {
            intake_voltage = 0;
        }

        // 2. OUTTAKE LOGIC
        if (intake_active && !outtake_active && !piston_stateB) {
            // SCENARIO 1: Intake is ON, but Outtake Toggle is OFF.
            outtake_voltage = -12000; 
        } 
        else if (outtake_active && !piston_stateB && intake_active) {
            // SCENARIO 2: Outtake Toggle is ON.
            // It follows L2 (Reverse if pressed, Forward if not).
            if (l2_pressed && !piston_stateB) {
                outtake_voltage = -12000;
            } else {
                outtake_voltage = 12000;
            }
        } 
        else if (intake_active && piston_stateB && !outtake_active) {
            // SCENARIO 3: piston down but outtake not on, keep blocks in on botton outwheel.
            outtake_voltage = 12000;
        }
        else if (outtake_active && piston_stateB && intake_active) {
            // SCENARIO 4: piston down and outtake on, reverse outwheel to output.
            //l2 reverse
            if (l2_pressed && piston_stateB) {
                outtake_voltage = -12000;
            } else {
                outtake_voltage = 300; // Slower forward to prevent throwing blocks too far when piston is down
            }
        }
        else {
            // SCENARIO 5: Outtake Toggle is OFF, and either Intake is OFF or piston is down.
            outtake_voltage = 0;
        }

        // 3. APPLY VOLTAGES
        intake_motor.move_voltage(intake_voltage);
        outtake_motor.move_voltage(outtake_voltage);

        pros::delay(ez::util::DELAY_TIME);                                     
    }
}