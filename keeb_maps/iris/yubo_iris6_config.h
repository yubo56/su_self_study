/*
Copyright 2021 Danny Nguyen <danny@keeb.io>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xCB10
#define PRODUCT_ID      0x6256
#define DEVICE_VER      0x0600
#define MANUFACTURER    "Keebio"
#define PRODUCT         "Iris Rev. 6"

/* key matrix size */
// Rows are doubled-up
#define MATRIX_ROWS 10
#define MATRIX_COLS 6

// wiring of each half
#define MATRIX_ROW_PINS { B1, F0, F5, B4, D7 }
#define MATRIX_COL_PINS { F1, F4, B5, C7, D4, D6 }
#define MATRIX_ROW_PINS_RIGHT { B1, F0, F5, B4, B5 }
#define MATRIX_COL_PINS_RIGHT { D4, D6, D7, C7, F1, F4 }
#define SPLIT_HAND_PIN D5

#define ENCODERS_PAD_A { B3 }
#define ENCODERS_PAD_B { B2 }
#define ENCODERS_PAD_A_RIGHT { B3 }
#define ENCODERS_PAD_B_RIGHT { B2 }

/* COL2ROW or ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* number of backlight levels */
// #define BACKLIGHT_LEVELS 3

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

/* serial.c configuration for split keyboard */
#define SOFT_SERIAL_PIN D0

/* ws2812 RGB LED */
#define RGB_DI_PIN E6
#ifdef RGB_DI_PIN
#    define RGBLED_NUM 68
#    define RGBLED_SPLIT { 34, 34 }
#    define RGBLIGHT_HUE_STEP 8
#    define RGBLIGHT_SAT_STEP 8
#    define RGBLIGHT_VAL_STEP 8
#    define RGBLIGHT_LIMIT_VAL 120 /* The maximum brightness level */
#    define RGBLIGHT_SLEEP         /* If defined, the RGB lighting will be switched off when the host goes to sleep */
/*== enabled animations ==*/
#    define ENABLE_RGB_MATRIX_BREATHING
#    define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#    define ENABLE_RGB_MATRIX_PIXEL_RAIN
#    define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#    define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#    define RGBLIGHT_DEFAULT_VAL 120
#    define RGBLIGHT_DEFAULT_MODE (RGBLIGHT_MODE_RAINBOW_SWIRL + 2)
// RGB Matrix
#    ifdef RGB_MATRIX_ENABLE
#        define RGB_MATRIX_MAXIMUM_BRIGHTNESS 120
#        define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#        define DRIVER_LED_TOTAL RGBLED_NUM
#        define RGB_MATRIX_SPLIT { 34, 34 }
#        define RGB_MATRIX_LED_COUNT 68
#        define RGB_DISABLE_WHEN_USB_SUSPENDED
#        define MASTER_LEFT
#        define RGB_MATRIX_KEYPRESSES
#        define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#    endif
#endif
