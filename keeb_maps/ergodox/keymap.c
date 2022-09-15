#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"
#include "keymap_hungarian.h"
#include "keymap_swedish.h"
#include "keymap_br_abnt2.h"
#include "keymap_canadian_multilingual.h"
#include "keymap_german_ch.h"
#include "keymap_jp.h"
#include "keymap_korean.h"
#include "keymap_bepo.h"
#include "keymap_italian.h"
#include "keymap_slovenian.h"
#include "keymap_lithuanian_azerty.h"
#include "keymap_danish.h"
#include "keymap_norwegian.h"
#include "keymap_portuguese.h"
#include "keymap_contributions.h"
#include "keymap_czech.h"
#include "keymap_romanian.h"
#include "keymap_russian.h"
#include "keymap_uk.h"
#include "keymap_estonian.h"
#include "keymap_belgian.h"
#include "keymap_us_international.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)
#define NO_PIPE_ALT KC_GRAVE
#define NO_BSLS_ALT KC_EQUAL
#define LSA_T(kc) MT(MOD_LSFT | MOD_LALT, kc)
#define BP_NDSH_MAC ALGR(KC_8)

enum custom_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
  HSV_172_255_255,
  HSV_86_255_128,
  HSV_27_255_255,
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_ergodox_pretty(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_UP, KC_DOWN, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_LEFT, KC_RGHT, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, MO(1), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_VOLD, KC_VOLU, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_NO, KC_NO, KC_NO, MO(4), KC_LGUI, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, TG(2), KC_NO, MO(4)),
	[1] = LAYOUT_ergodox_pretty(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MINS, KC_EQL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_RGHT, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_TRNS, KC_HOME, KC_F11, KC_END, KC_GRV, KC_PGUP, KC_DEL, KC_BSPC, KC_NO, KC_DOWN, KC_QUOT, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F6, KC_PGDN, KC_NO, KC_NO, KC_LEFT, KC_NO, KC_F8, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
	[2] = LAYOUT_ergodox_pretty(KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_UP, KC_DOWN, KC_6, KC_7, KC_8, KC_9, KC_0, KC_NO, KC_TAB, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y, KC_LEFT, KC_RGHT, KC_F, KC_G, KC_C, KC_R, KC_L, KC_BSLS, MO(3), KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S, KC_ENT, KC_LSFT, KC_SCLN, KC_Q, KC_J, KC_K, KC_X, KC_VOLD, KC_VOLU, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RSFT, KC_NO, KC_NO, KC_NO, MO(4), KC_LGUI, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_MUTE, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_TRNS, KC_NO, MO(4)),
	[3] = LAYOUT_ergodox_pretty(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_NO, KC_NO, KC_NO, KC_RGHT, KC_NO, KC_NO, KC_SLSH, KC_EQL, KC_NO, KC_TRNS, KC_HOME, KC_F11, KC_END, KC_ESC, KC_PGUP, KC_DEL, KC_BSPC, KC_NO, KC_DOWN, KC_MINS, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_F6, KC_PGDN, KC_NO, KC_NO, KC_LEFT, KC_NO, KC_F8, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_LGUI, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),
	[4] = LAYOUT_ergodox_pretty(KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_UP, KC_DOWN, KC_NO, KC_NO, KC_NO, KC_NO, KC_LSFT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LEFT, KC_RGHT, KC_NO, KC_NO, KC_NO, KC_RSFT, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_LGUI, KC_LCTL, KC_LALT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_LCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS)
};


bool suspended = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

uint32_t layer_state_set_user(uint32_t state) {

    uint8_t layer = biton32(state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      case 1:
        ergodox_right_led_1_on();
        break;
      case 2:
        ergodox_right_led_2_on();
        break;
      case 3:
        ergodox_right_led_3_on();
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        break;
      case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
      default:
        break;
    }
    return state;

};

void keyboard_post_init_user(void) {
  layer_state_set_user(layer_state);
}

