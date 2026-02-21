#pragma once
#include "liblvgl/lvgl.h"

// ==========================================
// IMAGE DECLARATIONS
// ==========================================
// We wrap these in extern "C" so the C++ compiler 
// can correctly find the variables defined in your .c files.
#ifdef __cplusplus
extern "C" {
#endif

// --- Main Menu (2 Rows x 4 Cols) ---
LV_IMAGE_DECLARE(img_2x4_blank);
LV_IMAGE_DECLARE(img_2x4_clawbot_button);
LV_IMAGE_DECLARE(img_2x4_controls_button);
LV_IMAGE_DECLARE(img_2x4_debatable);
LV_IMAGE_DECLARE(img_2x4_devices_button);
LV_IMAGE_DECLARE(img_2x4_drive_button);
LV_IMAGE_DECLARE(img_2x4_gray_discriptor);
LV_IMAGE_DECLARE(img_2x4_kira);
LV_IMAGE_DECLARE(img_2x4_match_button);
LV_IMAGE_DECLARE(img_2x4_motors_button);
LV_IMAGE_DECLARE(img_2x4_practise2);
LV_IMAGE_DECLARE(img_2x4_run_button);
LV_IMAGE_DECLARE(img_2x4_settings_button);
LV_IMAGE_DECLARE(img_2x4_slot_4);
LV_IMAGE_DECLARE(img_2x4_slot_5);
LV_IMAGE_DECLARE(img_2x4_slot_6);
LV_IMAGE_DECLARE(img_2x4_slot_7);
LV_IMAGE_DECLARE(img_2x4_slot_8);
LV_IMAGE_DECLARE(img_2x4_slot_program_type_desc);
LV_IMAGE_DECLARE(img_2x4_timed_run_button);
LV_IMAGE_DECLARE(img_2x4_user_folder);
LV_IMAGE_DECLARE(img_2x4_vex_folder);
LV_IMAGE_DECLARE(img_2x4_wiring_button);

// --- Devices Screen (4 Rows x 7 Cols) ---
LV_IMAGE_DECLARE(img_4x7_3_wire);
LV_IMAGE_DECLARE(img_4x7_back_alpha);
LV_IMAGE_DECLARE(img_4x7_battery);
LV_IMAGE_DECLARE(img_4x7_brain);
LV_IMAGE_DECLARE(img_4x7_controller_1);
LV_IMAGE_DECLARE(img_4x7_motor_1);
LV_IMAGE_DECLARE(img_4x7_motor_11);
LV_IMAGE_DECLARE(img_4x7_motor_12);
LV_IMAGE_DECLARE(img_4x7_motor_13);
LV_IMAGE_DECLARE(img_4x7_motor_2);
LV_IMAGE_DECLARE(img_4x7_motor_3);
LV_IMAGE_DECLARE(img_4x7_motor_4);
LV_IMAGE_DECLARE(img_4x7_motor_5);
LV_IMAGE_DECLARE(img_4x7_radio_21);

#ifdef __cplusplus
}
#endif

// ==========================================
// UI SYSTEM
// ==========================================

enum SceneID {
    SCENE_MAIN_MENU,
    SCENE_DEVICES,
    SCENE_SETTINGS,
    SCENE_USER_PROG,
    SCENE_VEX_PROG,
    SCENE_USER_FOLDER,
    SCENE_VEX_FOLDER
};

void ui_init_styles();
void ui_load_scene(SceneID scene);