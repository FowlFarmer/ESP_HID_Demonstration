#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tusb.h"

/**
 * @brief Initialize tinyusb HID device.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_ERR_NO_MEM: No memory
 */
esp_err_t tinyusb_hid_init(void);

/**
 * @brief Report key press in the keyboard, using array here,
 *        if open CONFIG_ENABLE_FULL_KEY_KEYBOARD contains all keys at once
 *        else contains six keys at most.
 *
 * @param keycode hid keyboard code array
 */
void tinyusb_hid_keyboard_report(uint8_t modifier, uint8_t keycode[]);
#ifdef __cplusplus
}
#endif
