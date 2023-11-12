#include QMK_KEYBOARD_H
#include "version.h"

enum layers {
    BASE,  // default layer
    SYMB,  // symbols
    MDIA,  // media keys
};

enum custom_keycodes {
#ifdef ORYX_CONFIGURATOR
    VRSN = EZ_SAFE_RANGE,
#else
    VRSN = SAFE_RANGE,
#endif
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_ergodox_pretty(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_UP, KC_DOWN, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_LEFT, KC_RGHT, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_VOLD, KC_VOLU, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_NO, KC_NO, KC_NO, MO(4), KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, TG(2), KC_NO, KC_LCTL),
	[1] = LAYOUT_ergodox_pretty(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MINS, KC_EQL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_RGHT, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_TRNS, KC_HOME, KC_F11, KC_END, KC_GRV, KC_PGUP, KC_DEL, KC_BSPC, KC_NO, KC_DOWN, KC_QUOT, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F6, KC_PGDN, KC_NO, KC_NO, KC_LEFT, KC_NO, KC_F8, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL),
	[2] = LAYOUT_ergodox_pretty(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_UP, KC_DOWN, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y, KC_LEFT, KC_RGHT, KC_F, KC_G, KC_C, KC_R, KC_L, KC_BSLS, MO(3), KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S, KC_ENT, KC_LSFT, KC_SCLN, KC_Q, KC_J, KC_K, KC_X, KC_VOLD, KC_VOLU, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RSFT, KC_NO, KC_NO, KC_NO, MO(4), KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_LCTL),
	[3] = LAYOUT_ergodox_pretty(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_RGHT, KC_NO, KC_NO, KC_SLSH, KC_EQL, KC_NO, KC_TRNS, KC_HOME, KC_F11, KC_END, KC_ESC, KC_PGUP, KC_DEL, KC_BSPC, KC_NO, KC_DOWN, KC_MINS, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F6, KC_PGDN, KC_NO, KC_NO, KC_LEFT, KC_NO, KC_F8, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL),
	[4] = LAYOUT_ergodox_pretty(KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL)
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
        }
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case 0:
#ifdef RGBLIGHT_COLOR_LAYER_0
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
            break;
        case 1:
            ergodox_right_led_1_on();
#ifdef RGBLIGHT_COLOR_LAYER_1
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
            break;
        case 2:
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_2
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
#endif
            break;
        case 3:
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_3
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
            break;
        case 4:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_4
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
            break;
        case 5:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_5
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
            break;
        case 6:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_6
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
#endif
            break;
        case 7:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_7
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
            break;
        default:
            break;
    }

    return state;
};
