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
        case 6: d_c_ctrl(); break; 
        case 7: flip_auton(); break;
    }
}

void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    
    // --- TOGGLES ---
    bool intake_active = false; 
    bool outtake_active = false;
    bool piston_stateA = false; 
    bool piston_stateB = false;
    bool piston_stateH = false;
    bool l2_pressed = false;

    //controller print rate adjust because pros is wack.
    //imput storage system sim.
    std::string last_printed = "";
    uint32_t last_print_time = 0;
    bool pending_print = false;
    std::string pending_text = "";
    int pending_row = 0;
    int pending_col = 0;

    auto schedule_print = [&](int row, int col, const std::string &msg) {
        uint32_t now = pros::millis();
        if (msg != last_printed && (now - last_print_time) >= 50) {
            master.clear_line(row);
            master.print(row, col, "%s", msg.c_str());
            last_printed = msg;
            last_print_time = pros::millis();
            pending_print = false;
        } else if (msg != last_printed) {
            pending_print = true;
            pending_text = msg;
            pending_row = row;
            pending_col = col;
        }
    };

    while (true) {
        //print send (flushes pending print if it's time, otherwise waits to print until 50ms have passed since last print to avoid flooding the controller) bro why is vs autocomplete goated
        if (pending_print) {
            uint32_t now = pros::millis();
            if ((now - last_print_time) >= 50) {
                master.clear_line(pending_row);
                master.print(pending_row, pending_col, "%s", pending_text.c_str());
                last_printed = pending_text;
                last_print_time = pros::millis();
                pending_print = false;
            }
        }

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
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN) && !l2_pressed) {
            piston_stateH = !piston_stateH;
            pneumH.set_value(piston_stateH);
        } else {
            // let auton selector do its own thing with the down button, don't let it interfere with opcontrol
        }

        // --- MOTOR LOGIC ---
        int intake_voltage = 0;
        int outtake_voltage = 0;
        l2_pressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        
        //INTAKE LOGIC
        if (intake_active) {
            if (l2_pressed) {
                intake_voltage = -12000; // Reverse
            } else {
                intake_voltage = 12000;  // Forward
            }
        } else {
            intake_voltage = 0;
        }

        //OUTTAKE LOGIC
        if (intake_active && !outtake_active && !piston_stateB) {
            //1. backspin outtake to store
            outtake_voltage = -12000; 
            schedule_print(0,0,"I/");
        } 
        else if (outtake_active && !piston_stateB && intake_active) {
            //2. only allow outtake while intaking.
            outtake_voltage = 12000;
            schedule_print(0,0,"I/OUTTAKE");
        } 
        else if (intake_active && piston_stateB && !outtake_active) {
            //3. intake with piston, outtake foward to store
            outtake_voltage = 12000;
            schedule_print(0,0,"I/PISTON");
        }
        else if (outtake_active && piston_stateB && intake_active) {
            //4. all on, outtake reverse to score
            outtake_voltage = -300;
            schedule_print(0,0,"I/OUTTAKE PISTON");
        }
        else {
            //5. outtake or piston on without intake = no outtake.
            outtake_voltage = 0;
            schedule_print(0,0,"IDLE");
        }

        // 3. APPLY VOLTAGES
        intake_motor.move_voltage(intake_voltage);
        outtake_motor.move_voltage(outtake_voltage);

        pros::delay(ez::util::DELAY_TIME);                                     
    }
}