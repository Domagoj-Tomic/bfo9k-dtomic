#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

enum {
    TD_ESC_GRV = 0
};

static bool last_autoshift_state = true;

/*-------------------------------------------------------------------------------------------------*/

void set_lighting_for_autoshift(bool enabled) {
    rgblight_enable();
    rgblight_mode(RGBLIGHT_MODE_STATIC_LIGHT);
    if (enabled) {
        rgblight_sethsv_noeeprom(0, 0, 255);       // White (Auto Shift ON)
    } else {
        rgblight_sethsv_noeeprom(191, 255, 255);   // Purple (Auto Shift OFF)
    }
}

void matrix_scan_user(void) {
    bool cur_state = get_autoshift_state();
    if (cur_state != last_autoshift_state) {
        set_lighting_for_autoshift(cur_state);
        last_autoshift_state = cur_state;
    }
}

void suspend_power_down_user(void) {
    rgblight_disable();
}

void suspend_wakeup_init_user(void) {
    rgblight_enable();
    set_lighting_for_autoshift(get_autoshift_state());
}

void esc_grv_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (state->pressed) {           // Tap
            tap_code(KC_GRV);
        } else {                        // Hold
            register_code(KC_ESC);
            wait_ms(20);
            unregister_code(KC_ESC);
        }
    }
}

void esc_grv_reset(tap_dance_state_t *state, void *user_data) {}

tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_grv_finished, esc_grv_reset)
};

/*-------------------------------------------------------------------------------------------------*/

void keyboard_post_init_user(void) {
    set_lighting_for_autoshift(get_autoshift_state());

    if (!host_keyboard_led_state().num_lock) {
        tap_code(KC_NUM_LOCK);
    }
    if (host_keyboard_led_state().scroll_lock) {
        tap_code(KC_SCROLL_LOCK);
    }
}

// Short aliases for better formatting
#define TD_EG    TD(TD_ESC_GRV)
#define AS_TG    QK_AUTO_SHIFT_TOGGLE
#define ___      KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        TD_EG,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,              KC_PGUP, KC_HOME, KC_F9,    KC_F10,  KC_F11,  KC_F12,  KC_PSCR, AS_TG,   KC_VOLU,

        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,               KC_PGDN, KC_END,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_PAUS, KC_NO,   KC_VOLD,

        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS, KC_SCLN, KC_0,               KC_P7,   KC_P8,   KC_P9,    KC_PSLS, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,

        CW_TOGG, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_EQL,  KC_QUOT, KC_BSLS,            KC_P4,   KC_P5,   KC_P6,    KC_PAST, KC_H,    KC_J,    KC_K,    KC_L,    KC_BSPC,

        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_SLSH, KC_LBRC, KC_RBRC,            KC_P1,   KC_P2,   KC_P3,    KC_PMNS, KC_N,    KC_M,    KC_UP,   KC_INS,  KC_DEL,

        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,  KC_SPC,  KC_SPC,  KC_COMM, KC_DOT,             KC_ENT,  KC_P0,   KC_PDOT,  KC_PPLS, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT, KC_ENT
    )
};

#ifdef OTHER_KEYMAP_C
#    include OTHER_KEYMAP_C
#endif // OTHER_KEYMAP_C
