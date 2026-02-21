#include "main.h"
#include "auton_select.hpp"
#include "autons.hpp"
#include "globals.hpp"

void initialize() {
    // 1. Initialize EZ-Template
    chassis.initialize();
    chassis.opcontrol_drive_activebrake_set(0); 

    // 2. Create the Selector
    create_auton_selector();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    // Reset PID/Sensors
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
    bool intake_active = false; 
    bool piston_state = false; // false = Retracted, true = Extended
    bool outtake_active = false;

    // Coast for driver control
    chassis.drive_brake_set(MOTOR_BRAKE_COAST);

    // --- MANUAL MOTOR DEFINITIONS ---
    // We define these here to manually control them, bypassing EZ-Template for Driver Control
    // Ensure these ports match what is in src/config/globals.cpp!
    static pros::MotorGroup left_mg({-1, -2, -3});
    static pros::MotorGroup right_mg({4, 5, 6});

    while (true) {
        // --- SAFETY CHECK ---
        // If the debug run is active, SKIP control to prevent fighting the auton
        if (is_debug_running) {
            pros::delay(20);
            continue;
        }
        // --- DRIVE CONTROL ---
        int power = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int turn  = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        left_mg.move(power + turn);
        right_mg.move(power - turn);
        
        // --- INTAKE LOGIC ---
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            intake_active = !intake_active; 
        }
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            outtake_active = !outtake_active; 
        } 
        //---------------------------------//
        int intake_voltage = 0;
        int outtake_voltage = 0;
        if (intake_active) {
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
                intake_voltage = -12000;
            } else {
                intake_voltage = 12000;
            }
        } else {
            intake_voltage = 0;
        }
        if (outtake_active) {
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
                outtake_voltage = -12000;
            } else {
                outtake_voltage = 12000;
            }
        } else {
            outtake_voltage = 0;
        }
        outtake_motor.move_voltage(outtake_voltage);
        intake_motor.move_voltage(intake_voltage);
        // --- PNEUMATIC LOGIC ---
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
            piston_state = !piston_state; // Flip the state
            pneumA.set_value(piston_state); // Apply: true = Extend, false = Retract
        }

        pros::delay(ez::util::DELAY_TIME);                                     
    }
}