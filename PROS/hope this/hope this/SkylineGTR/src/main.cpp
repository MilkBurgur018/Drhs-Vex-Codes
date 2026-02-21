#include "main.h"
#include "ui_system.hpp"

// ==========================================
// VEX DEVICES
// ==========================================
pros::MotorGroup left_mg({-1, -4, -11});      
pros::MotorGroup right_mg({3, 5, 13});  
pros::MotorGroup Hdrive_mg({2, -12});  

void initialize() {
    ui_init_styles();
    
    // Start at Main Menu
    // FIX: Use SCENE_MAIN_MENU instead of LAYOUT_MAIN_MENU
    ui_load_scene(SCENE_MAIN_MENU);
}

void disabled() {}
void competition_initialize() {}
void autonomous() {}

void opcontrol() {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    
    left_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    right_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    Hdrive_mg.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    while (true) {
        // Arcade control
        int dir = master.get_analog(ANALOG_LEFT_Y);    
        int turn = master.get_analog(ANALOG_RIGHT_X);  
        int strafe = master.get_analog(ANALOG_LEFT_X);  
        
        left_mg.move(dir - turn);                      
        right_mg.move(dir + turn);                     
        Hdrive_mg.move(strafe);                        

        if(master.get_digital(DIGITAL_R1)) {
            left_mg.brake();
            right_mg.brake();
            Hdrive_mg.brake();
        }
        
        // Debug: Reset to Main Menu if Button A is pressed
        if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
             ui_load_scene(SCENE_MAIN_MENU);
        }

        pros::delay(20);
    }
}