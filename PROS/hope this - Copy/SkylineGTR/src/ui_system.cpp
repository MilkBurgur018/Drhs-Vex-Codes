#include "ui_system.hpp"
#include "main.h"

// Track current scene for navigation logic
static SceneID current_scene_id = SCENE_MAIN_MENU;

// ==========================================
// NAVIGATION LOGIC
// ==========================================
static int get_target_scene(SceneID current, int btn_index) {
    switch (current) {
        case SCENE_MAIN_MENU:
            if (btn_index == 0) return SCENE_VEX_PROG;    
            if (btn_index == 1) return SCENE_DEVICES;     
            if (btn_index == 2) return SCENE_SETTINGS;    
            if (btn_index == 3) return SCENE_VEX_FOLDER;  
            if (btn_index == 4) return SCENE_USER_PROG;   
            if (btn_index == 5) return SCENE_USER_PROG;   
            if (btn_index == 6) return SCENE_USER_PROG;   
            if (btn_index == 7) return SCENE_USER_FOLDER; 
            break;
        case SCENE_USER_PROG:
            return SCENE_MAIN_MENU; 
        case SCENE_VEX_PROG:
            if (btn_index == 6) return SCENE_DEVICES;
            return SCENE_MAIN_MENU;
        case SCENE_USER_FOLDER:
            if (btn_index == 0) return SCENE_USER_PROG; 
            if (btn_index == 1) return SCENE_USER_PROG; 
            if (btn_index == 2) return SCENE_USER_PROG; 
            break;
        case SCENE_VEX_FOLDER:
            if (btn_index == 0) return SCENE_VEX_PROG; 
            break;
        case SCENE_DEVICES:
             return SCENE_MAIN_MENU;
             break;
        default: break;
    }
    return -1; 
}

// ==========================================
// STYLES & CALLBACKS
// ==========================================
static lv_style_t style_btn_default;
static lv_style_t style_btn_pressed;

void ui_init_styles() {
    lv_style_init(&style_btn_default);
    lv_style_set_radius(&style_btn_default, 0);
    lv_style_set_border_width(&style_btn_default, 0);
    lv_style_set_bg_opa(&style_btn_default, LV_OPA_TRANSP);
    
    lv_style_init(&style_btn_pressed);
    lv_style_set_image_recolor(&style_btn_pressed, lv_color_white());
    lv_style_set_image_recolor_opa(&style_btn_pressed, LV_OPA_50);
}

static void btn_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t* target_obj = (lv_obj_t*)lv_event_get_target(e);
        int btn_id = (int)(intptr_t)lv_obj_get_user_data(target_obj);
        int target_scene = get_target_scene(current_scene_id, btn_id);
        if (target_scene != -1) {
            ui_load_scene((SceneID)target_scene);
        }
    }
}

// ==========================================
// LAYOUT GENERATORS
// ==========================================
static lv_obj_t * current_scr = NULL;

void reset_screen_container() {
    if (current_scr) lv_obj_delete(current_scr);
    current_scr = lv_obj_create(lv_screen_active());
    lv_obj_set_size(current_scr, 480, 240);
    lv_obj_set_pos(current_scr, 0, 0);
    lv_obj_set_style_pad_all(current_scr, 0, 0);
    lv_obj_set_style_border_width(current_scr, 0, 0);
    lv_obj_set_style_bg_color(current_scr, lv_color_black(), 0);
}

void create_2x4_scene(const lv_image_dsc_t* images[8]) {
    int w = 480 / 4; 
    int h = 240 / 2; 

    for (int i = 0; i < 8; i++) {
        int col = i % 4;
        int row = i / 4;
        lv_obj_t * btn = lv_button_create(current_scr);
        lv_obj_set_pos(btn, col * w, row * h);
        lv_obj_set_size(btn, w, h);
        lv_obj_add_style(btn, &style_btn_default, 0);
        lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);
        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

        if (images[i] != NULL) {
            lv_obj_t * img = lv_image_create(btn);
            lv_image_set_src(img, images[i]);
            lv_obj_center(img);
        }
    }
}

static int get_col_x(int col) { return (col == 0) ? 0 : 72 + (col - 1) * 68; }
static int get_col_width(int col) { return (col == 0) ? 72 : 68; }

void create_devices_scene() {
    lv_obj_t * bg = lv_image_create(current_scr);
    lv_image_set_src(bg, &img_4x7_back_alpha);
    lv_obj_set_pos(bg, 0, 0);

    int row_h = 60;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 7; col++) {
            int id = (row * 7) + col + 1;
            const lv_image_dsc_t* icon = NULL;
            
            if (id == 2) icon = &img_4x7_controller_1;
            else if (id == 3) icon = &img_4x7_brain;
            else if (id == 4) icon = &img_4x7_battery;
            else if (id == 5) icon = &img_4x7_3_wire;
            else if (id == 8) icon = &img_4x7_motor_1;
            else if (id == 9) icon = &img_4x7_motor_2;
            else if (id == 10) icon = &img_4x7_motor_3;
            else if (id == 11) icon = &img_4x7_motor_4;
            else if (id == 12) icon = &img_4x7_motor_5;
            else if (id == 18) icon = &img_4x7_motor_11;
            else if (id == 19) icon = &img_4x7_motor_12;
            else if (id == 20) icon = &img_4x7_motor_13;
            else if (id == 28) icon = &img_4x7_radio_21;

            if (icon != NULL) {
                int x = get_col_x(col);
                int y = row * row_h;
                int w = get_col_width(col);
                lv_obj_t * btn = lv_button_create(current_scr);
                lv_obj_set_pos(btn, x, y);
                lv_obj_set_size(btn, w, row_h);
                lv_obj_add_style(btn, &style_btn_default, 0);
                lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);
                lv_obj_set_user_data(btn, (void*)(intptr_t)id);
                lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);

                lv_obj_t * img = lv_image_create(btn);
                lv_image_set_src(img, icon);
                lv_obj_center(img);
            }
        }
    }
}

void ui_load_scene(SceneID scene) {
    current_scene_id = scene;
    reset_screen_container();

    if (scene == SCENE_SETTINGS) {
        lv_obj_t * label = lv_label_create(current_scr);
        lv_label_set_text(label, "Settings - TODO");
        lv_obj_center(label);
        lv_obj_t * btn = lv_button_create(current_scr);
        lv_obj_set_pos(btn, 0, 0);
        lv_obj_set_size(btn, 480, 240);
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
        lv_obj_add_event_cb(btn, [](lv_event_t* e){ ui_load_scene(SCENE_MAIN_MENU); }, LV_EVENT_CLICKED, NULL);
        return;
    }
    
    if (scene == SCENE_DEVICES) {
        create_devices_scene();
        return;
    }

    const lv_image_dsc_t* imgs[8] = {NULL};
    if (scene == SCENE_MAIN_MENU) {
        imgs[0] = &img_2x4_drive_button;
        imgs[1] = &img_2x4_devices_button;
        imgs[2] = &img_2x4_settings_button;
        imgs[3] = &img_2x4_vex_folder;
        imgs[4] = &img_2x4_practise2;
        imgs[5] = &img_2x4_debatable;
        imgs[6] = &img_2x4_kira;
        imgs[7] = &img_2x4_user_folder;
    } 
    else if (scene == SCENE_USER_PROG) {
        imgs[0] = &img_2x4_run_button;
        imgs[1] = &img_2x4_timed_run_button;
        imgs[2] = &img_2x4_match_button;
        imgs[3] = &img_2x4_wiring_button;
        imgs[4] = &img_2x4_gray_discriptor;
        imgs[5] = &img_2x4_gray_discriptor;
        imgs[6] = &img_2x4_slot_program_type_desc;
        imgs[7] = &img_2x4_controls_button;
    }
    else if (scene == SCENE_VEX_PROG) {
        imgs[0] = &img_2x4_run_button;
        imgs[1] = &img_2x4_timed_run_button;
        imgs[2] = &img_2x4_match_button;
        imgs[3] = &img_2x4_wiring_button;
        imgs[4] = &img_2x4_gray_discriptor;
        imgs[5] = &img_2x4_gray_discriptor;
        imgs[6] = &img_2x4_motors_button;
        imgs[7] = &img_2x4_controls_button;
    }
    else if (scene == SCENE_USER_FOLDER) {
        imgs[0] = &img_2x4_practise2;
        imgs[1] = &img_2x4_debatable;
        imgs[2] = &img_2x4_kira;
        imgs[3] = &img_2x4_slot_4;
        imgs[4] = &img_2x4_slot_5;
        imgs[5] = &img_2x4_slot_6;
        imgs[6] = &img_2x4_slot_7;
        imgs[7] = &img_2x4_slot_8;
    }
    else if (scene == SCENE_VEX_FOLDER) {
        imgs[0] = &img_2x4_clawbot_button;
        for(int i=1; i<8; i++) imgs[i] = &img_2x4_blank;
    }
    create_2x4_scene(imgs);
}