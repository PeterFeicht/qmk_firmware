#pragma once

#define RGBLIGHT_LAYERS      // Enable lighting layers
#define RGBLIGHT_LAYER_BLINK // Enable lighting layer blink
#define RGBLIGHT_LAYERS_OVERRIDE_RGB_OFF // Show layers even when RGB is off otherwise

#define RGBLIGHT_SLEEP        // Enable rgblight_suspend() and rgblight_wakeup() in keymap.c
#define RGBLIGHT_TIMEOUT 3600 // Seconds to wait until rgblight time out

#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
