#include "main.h"
#include "liblvgl/lvgl.h"
#include "auton_select.hpp"
#include "autons.hpp" // <--- CRITICAL: This fixes "undefined identifier" errors

// --- GLOBALS ---
int selected_auton = 1; 
bool is_debug_running = false;
lv_obj_t * debug_label_ptr = NULL;

// store pointers to buttons so the controller can access them
lv_obj_t * btn_objs[8]; 

// --- STYLES ---
static lv_style_t style_btn_default;
static lv_style_t style_btn_checked;
static lv_style_t style_btn_focused; // Controller Hover

static lv_style_t style_debug_default;
static lv_style_t style_debug_checked;
static lv_style_t style_debug_focused; // Controller Hover

// --- LOGIC: DEBUG TASK ---
void debug_auton_task_fn(void* param) {
    printf("Debug run started!\n");
    // Switch calls the functions defined in autons.hpp
    switch (selected_auton) {
        case 1: left_qual_auton();   break;
        case 2: right_qual_auton();  break;
        case 3: skills_auton_for_qual(); break;
        case 4: right_qual_auton_center(); break;
        case 5: left_qual_auton_ram(); break;
        case 6: defensive_mode_auton(); break; // Added these so buttons 6 & 7  SIX SEVEN
        case 7: high_speed_mode_auton(); break;
        default: printf("No auton selected or fallback!\n"); break;
    }

    printf("Debug run finished!\n");
    if (debug_label_ptr != NULL) {
        lv_label_set_text(debug_label_ptr, "debug test run");
    }
    is_debug_running = false;
}

// --- LOGIC: SELECTION CORE ---
void execute_auton_selection(int btn_index) {
    if (btn_index < 7) {
        selected_auton = btn_index + 1;
    }

    // Uncheck all regular buttons
    for(int i = 0; i < 8; i++) {
        if (btn_index < 7 && i < 7) {
            lv_obj_clear_state(btn_objs[i], LV_STATE_CHECKED);
        }
    }

    // Check selected
    if (btn_index < 7) {
        lv_obj_add_state(btn_objs[btn_index], LV_STATE_CHECKED);
    }

    // Debug Logic
    if (btn_index == 7) {
        lv_obj_t * label = lv_obj_get_child(btn_objs[7], 0);
        lv_label_set_text(label, "running...");
        
        if (!is_debug_running) {
            is_debug_running = true;
            pros::Task debug_task(debug_auton_task_fn, NULL, "DebugTask");
        }
    } else {
        if(debug_label_ptr != NULL) lv_label_set_text(debug_label_ptr, "debug test run");
    }
}

// --- CALLBACK: TOUCHSCREEN ---
static void auton_click_cb(lv_event_t * e) {
    int btn_index = (int)(uintptr_t)lv_event_get_user_data(e);
    execute_auton_selection(btn_index);
}

// --- TASK: CONTROLLER WATCHER ---
void controller_watcher_task(void* param) {
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    int cursor_index = 0;
    bool was_l2_pressed = false;

    bool dpad_left_active = false;
    bool dpad_right_active = false;
    bool dpad_up_active = false;
    bool dpad_down_active = false;
    bool btn_a_active = false;

    while (true) {
        if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            
            if (!was_l2_pressed) {
                lv_obj_add_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                was_l2_pressed = true;
            }

            // Left/Right
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
                if (!dpad_left_active) {
                    lv_obj_clear_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    if (cursor_index % 4 == 0) cursor_index += 3; 
                    else cursor_index--;
                    lv_obj_add_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    dpad_left_active = true;
                }
            } else dpad_left_active = false;

            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
                if (!dpad_right_active) {
                    lv_obj_clear_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    if (cursor_index % 4 == 3) cursor_index -= 3; 
                    else cursor_index++;
                    lv_obj_add_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    dpad_right_active = true;
                }
            } else dpad_right_active = false;

            // Up/Down
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) || master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
                 if (!dpad_up_active && !dpad_down_active) {
                    lv_obj_clear_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    if (cursor_index < 4) cursor_index += 4;
                    else cursor_index -= 4;
                    lv_obj_add_state(btn_objs[cursor_index], LV_STATE_FOCUSED);
                    dpad_up_active = true; 
                 }
            } else { dpad_up_active = false; dpad_down_active = false; }

            // Select
            if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
                if (!btn_a_active) {
                    execute_auton_selection(cursor_index);
                    btn_a_active = true;
                }
            } else btn_a_active = false;

        } else {
            if (was_l2_pressed) {
                for(int i=0; i<8; i++) {
                    lv_obj_clear_state(btn_objs[i], LV_STATE_FOCUSED);
                }
                was_l2_pressed = false;
            }
        }
        pros::delay(20);
    }
}

void create_auton_selector() {
    // 1. Regular Buttons Style
    lv_style_init(&style_btn_default);
    lv_style_set_bg_color(&style_btn_default, lv_color_hex(0x575E57)); 
    lv_style_set_bg_opa(&style_btn_default, LV_OPA_COVER);
    lv_style_set_border_width(&style_btn_default, 4);
    lv_style_set_border_color(&style_btn_default, lv_color_hex(0x5B875B)); 
    lv_style_set_text_color(&style_btn_default, lv_color_white());

    lv_style_init(&style_btn_checked);
    lv_style_set_bg_color(&style_btn_checked, lv_color_hex(0x5B875B)); 
    lv_style_set_border_color(&style_btn_checked, lv_color_hex(0x575E57)); 
    lv_style_set_text_color(&style_btn_checked, lv_color_black());

    lv_style_init(&style_btn_focused);
    lv_style_set_bg_color(&style_btn_focused, lv_color_hex(0x7A857A)); 
    lv_style_set_border_color(&style_btn_focused, lv_color_white());    
    lv_style_set_border_width(&style_btn_focused, 5);

    // 2. Debug Button Style
    lv_style_init(&style_debug_default);
    lv_style_set_bg_color(&style_debug_default, lv_palette_main(LV_PALETTE_INDIGO));
    lv_style_set_bg_opa(&style_debug_default, LV_OPA_COVER);
    lv_style_set_border_width(&style_debug_default, 4);
    lv_style_set_border_color(&style_debug_default, lv_palette_lighten(LV_PALETTE_INDIGO, 3));
    lv_style_set_text_color(&style_debug_default, lv_color_white());

    lv_style_init(&style_debug_checked);
    lv_style_set_bg_color(&style_debug_checked, lv_palette_lighten(LV_PALETTE_INDIGO, 3));
    lv_style_set_border_color(&style_debug_checked, lv_palette_main(LV_PALETTE_INDIGO));
    lv_style_set_text_color(&style_debug_checked, lv_color_black());

    lv_style_init(&style_debug_focused);
    lv_style_set_bg_color(&style_debug_focused, lv_palette_lighten(LV_PALETTE_INDIGO, 2));
    lv_style_set_border_color(&style_debug_focused, lv_color_white());
    lv_style_set_border_width(&style_debug_focused, 5);

    // --- Container ---
    lv_obj_t * cont = lv_obj_create(lv_scr_act()); 
    lv_obj_set_size(cont, 480, 240);
    lv_obj_center(cont);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); 
    lv_obj_set_style_pad_all(cont, 5, 0);
    lv_obj_set_style_bg_opa(cont, 0, 0); 
    lv_obj_set_style_border_width(cont, 0, 0);

    // Grid Layout for LVGL 8.3
    static lv_coord_t column_dsc[] = {112, 112, 112, 112, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {108, 108, LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_grid_dsc_array(cont, column_dsc, row_dsc);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID); 

    lv_obj_set_style_pad_column(cont, 8, 0); 
    lv_obj_set_style_pad_row(cont, 8, 0);

    // --- Buttons ---
    for(int i = 0; i < 8; i++) {
        lv_obj_t * btn = lv_btn_create(cont);
        
        // Grid Cell Placement
        lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, i % 4, 1, 
                                  LV_GRID_ALIGN_STRETCH, i / 4, 1);
        
        lv_obj_add_event_cb(btn, auton_click_cb, LV_EVENT_CLICKED, (void*)(uintptr_t)i);
        btn_objs[i] = btn;

        // Styles
        if (i == 7) { 
            lv_obj_add_style(btn, &style_debug_default, 0);
            lv_obj_add_style(btn, &style_debug_checked, LV_STATE_CHECKED);
            lv_obj_add_style(btn, &style_debug_focused, LV_STATE_FOCUSED);
        } else {
            lv_obj_add_style(btn, &style_btn_default, 0);
            lv_obj_add_style(btn, &style_btn_checked, LV_STATE_CHECKED);
            lv_obj_add_style(btn, &style_btn_focused, LV_STATE_FOCUSED);
        }

        // Check the default one
        if(i == 0) lv_obj_add_state(btn, LV_STATE_CHECKED);
        
        // Label
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(label, 100); 
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(label); 

        if (i == 7) debug_label_ptr = label; 

        switch (i) {
            case 0: lv_label_set_text(label, "Left Qual Auton"); break;
            case 1: lv_label_set_text(label, "Right Qual Auton"); break;
            case 2: lv_label_set_text(label, "Skills Auton Qual"); break;
            case 3: lv_label_set_text(label, "Right Qual Center"); break;
            case 4: lv_label_set_text(label, "Left Qual Ram"); break;
            case 5: lv_label_set_text(label, "Defensive Mode"); break;
            case 6: lv_label_set_text(label, "High Speed Mode"); break;
            case 7: lv_label_set_text(label, "debug test run"); break;
        }
    }

    pros::Task controller_task(controller_watcher_task, NULL, "ControllerMonitor");
}