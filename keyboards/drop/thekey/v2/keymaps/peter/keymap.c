/*
 * Keymap features:
 *  - Lock screen on SO key tap (Win/Super/GUI + L)
 *  - RGB light configuration on SO key hold
 *  - Copy/paste on C/V key tap
 *  - Custom strings on C/V key hold
 *  - Automatic RGB timeout on inactivity
 *
 * Copyright 2025 Peter Feichtinger <shippo@gmx.at>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H

#define HSV_WHITE50 0, 0, 127
#define STRING_C "My string."
#define STRING_V "My other string!"

enum custom_keycodes {
    LAYER_SWITCH = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Default */
    [0] = LAYOUT(LT(0, KC_L), LT(0, KC_C), LT(0, KC_V)),

    /* RGB Toggle + Mode Change */
    [1] = LAYOUT(LAYER_SWITCH, QK_UNDERGLOW_TOGGLE, QK_UNDERGLOW_MODE_NEXT),

    /* RGB Brightness */
    [2] = LAYOUT(LAYER_SWITCH, QK_UNDERGLOW_VALUE_DOWN, QK_UNDERGLOW_VALUE_UP),

    /* RGB Hue */
    [3] = LAYOUT(LAYER_SWITCH, QK_UNDERGLOW_HUE_DOWN, QK_UNDERGLOW_HUE_UP),

    /* RGB Saturation */
    [4] = LAYOUT(LAYER_SWITCH, QK_UNDERGLOW_SATURATION_DOWN, QK_UNDERGLOW_SATURATION_UP),
};

/* Lighting layers */

const rgblight_segment_t PROGMEM layer_indicator_0[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 5, HSV_WHITE50}
);
const rgblight_segment_t PROGMEM layer_indicator_1[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_OFF},
    {2, 1, HSV_WHITE50},
    {3, 2, HSV_OFF}
);
const rgblight_segment_t PROGMEM layer_indicator_2[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 3, HSV_OFF},
    {3, 1, HSV_WHITE50},
    {4, 1, HSV_OFF}
);
const rgblight_segment_t PROGMEM layer_indicator_3[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 4, HSV_OFF},
    {4, 1, HSV_WHITE50}
);
const rgblight_segment_t PROGMEM layer_indicator_4[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_WHITE50},
    {1, 4, HSV_OFF}
);

const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    layer_indicator_0,
    layer_indicator_1,
    layer_indicator_2,
    layer_indicator_3,
    layer_indicator_4
);

void keyboard_post_init_user(void) {
    /* Enable the LED layers */
    rgblight_layers = rgb_layers;
}

/* Layer handler */

static void next_layer(void);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(0, KC_L):
            if (record->tap.count && record->event.pressed) {
                // SO key tap -> Lock
                tap_code16(LWIN(KC_L));
            } else if (record->event.pressed) {
                // SO key hold -> Enter setup layer
                next_layer();
            }
            return false;

        case LT(0, KC_C):
            if (record->tap.count && record->event.pressed) {
                // SO key tap -> Copy
                tap_code16(C(KC_C));
            } else if (record->event.pressed) {
                // SO key hold -> Enter string C
                SEND_STRING(STRING_C);
            }
            return false;

        case LT(0, KC_V):
            if (record->tap.count && record->event.pressed) {
                // SO key tap -> Paste
                tap_code16(C(KC_V));
            } else if (record->event.pressed) {
                // SO key hold -> Enter string V
                SEND_STRING(STRING_V);
            }
            return false;

        case LAYER_SWITCH:
            if (record->event.pressed) {
                next_layer();
            }
            return false;

        default:
            return true;
    }
}

uint16_t layer = 0;

void next_layer(void) {
    if (layer > 0) {
        layer_off(layer);
    }

    rgblight_unblink_layer(layer);
    layer = (layer + 1) % 5;
    rgblight_blink_layer_repeat(layer, 1000, 1);

    if (layer > 0) {
        layer_on(layer);
    }
}

/* RGB timeout */

bool rgb_timeout = false;

void housekeeping_task_user(void) {
#if RGBLIGHT_TIMEOUT > 0
    if (last_input_activity_elapsed() > (uint32_t)(RGBLIGHT_TIMEOUT) * 1000) {
        if (!rgb_timeout) {
            rgb_timeout = true;
            rgblight_suspend();
        }
    } else {
        if (rgb_timeout) {
            rgb_timeout = false;
            rgblight_wakeup();
        }
    }
#endif
}
