#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "tinyusb.h"
//#include "tusb_hid.h"

#define BUTTON_GPIO GPIO_NUM_4

static const char *TAG = "USB_HID_Keyboard";
static bool button_pressed = false;
void send_key(uint8_t key) {
    uint8_t report[8] = {0}; // HID report buffer
    report[2] = key; // Set key code
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report);
    vTaskDelay(pdMS_TO_TICKS(100));
    report[2] = 0; // Release key
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, report);
}
void app_main(void) {
    esp_err_t ret;

    // Initialize NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize USB HID
    tinyusb_driver_install(NULL);

    while(1){
        send_key(0x04);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//void button_isr_handler(void *arg) {
//     button_pressed = true;
// }

// void button_task(void *arg) {
//     gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
//     gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
//     gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);
//     gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

//     while (1) {
//         if (button_pressed) {
//             ESP_LOGI(TAG, "Button pressed, sending 'a' key");
//             send_key(HID_KEY_A);
//             button_pressed = false;
//         }
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }




    // // Initialize GPIO ISR
    // gpio_install_isr_service(0);

    // // Start button task
    // xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);


