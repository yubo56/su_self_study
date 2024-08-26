// Copyright 2023 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 3

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT
(
    KC_ESC  , KC_1 , KC_2 , KC_3 , KC_4     , KC_5                        , KC_6    , KC_7  , KC_8    , KC_9   , KC_0    , KC_NO   ,
    KC_TAB  , KC_Q , KC_W , KC_E , KC_R     , KC_T                        , KC_Y    , KC_U  , KC_I    , KC_O   , KC_P    , KC_BSLS ,
    MO(1)   , KC_A , KC_S , KC_D , KC_F     , KC_G                        , KC_H    , KC_J  , KC_K    , KC_L   , KC_SCLN , KC_ENT  ,
    KC_LSFT , KC_Z , KC_X , KC_C , KC_V     , KC_B    , KC_MUTE , KC_NO   , KC_N    , KC_M  , KC_COMM , KC_DOT , KC_SLSH , KC_RSFT ,
                                      MO(2) , KC_LGUI , KC_SPC  , KC_LCTL , KC_LALT , MO(3)
),
    [1] = LAYOUT
(
    KC_NO   , KC_NO   , KC_NO  , KC_NO  , KC_NO   , KC_NO                      , KC_NO   , KC_NO   , KC_NO , KC_MINS , KC_EQL  , KC_NO   ,
    KC_NO   , KC_NO   , KC_NO  , KC_NO  , KC_UP   , KC_NO                      , KC_RGHT , KC_NO   , KC_NO , KC_LBRC , KC_RBRC , KC_NO   ,
    KC_TRNS , KC_HOME , KC_F11 , KC_END , KC_GRV  , KC_PGUP                    , KC_DEL  , KC_BSPC , KC_NO , KC_DOWN , KC_QUOT , KC_NO   ,
    KC_LSFT , KC_NO   , KC_NO  , KC_NO  , KC_F6   , KC_PGDN    , KC_NO , KC_NO , KC_LEFT , KC_NO   , KC_F8 , KC_NO   , KC_NO   , KC_RSFT ,
                                            KC_NO , KC_LGUI    , KC_NO , KC_NO , KC_RCTL , KC_RALT
),
    [2] = LAYOUT
(
    KC_NO   , KC_F1 , KC_F2 , KC_F3 , KC_F4      , KC_F5                      , KC_F6   , KC_F7    , KC_F8 , KC_F9  , KC_F10 , KC_NO ,
    KC_NO   , KC_NO , KC_NO , KC_NO , KC_NO      , KC_NO                      , KC_NO   , KC_NO    , KC_NO , KC_F11 , KC_F12 , KC_NO ,
    KC_NO   , KC_NO , KC_NO , KC_NO , KC_NO      , KC_NO                      , KC_NO   , KC_NO    , KC_NO , KC_NO  , KC_NO  , KC_NO ,
    KC_LSFT , KC_NO , KC_NO , KC_NO , KC_NO      , KC_NO                      , KC_NO   , KC_NO    , KC_NO , KC_NO  , KC_NO  , KC_NO , KC_NO , KC_RSFT ,
                                         KC_TRNS , KC_LGUI  , KC_LCTL , KC_NO , KC_RCTL , KC_RALT
) ,
    [3] = LAYOUT
(
    KC_NO , RGB_TOG , RGB_RMOD , RGB_MOD , RGB_SPD , RGB_SPI                        , RGB_SAD , RGB_SAI , KC_NUM_LOCK , KC_PSLS , KC_PAST , KC_NO   ,
    KC_NO , RGB_VAD , RGB_VAI  , RGB_HUD , RGB_HUI , RGB_SPD                        , RGB_SPI , KC_NO   , KC_P7       , KC_P8   , KC_P9   , KC_PMNS ,
    KC_NO , KC_NO   , KC_NO    , KC_NO   , KC_NO   , KC_NO                          , KC_UP   , KC_DOWN , KC_P4       , KC_P5   , KC_P6   , KC_PPLS , KC_NO ,
    KC_NO , QK_BOOT , KC_NO    , KC_NO   , KC_NO   , KC_NO      , KC_TRNS , KC_LEFT , KC_RGHT , KC_P1   , KC_P2       , KC_P3   , KC_PEQL ,
                                             KC_NO , KC_NO      , KC_NO   , KC_TRNS , KC_VOLD , KC_VOLU
)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}
