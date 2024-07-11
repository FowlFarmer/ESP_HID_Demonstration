#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_private/usb_phy.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <string.h>
#include "tinyusb_hid.h"
#ifdef CONFIG_ESP32_S3_USB_OTG
#include "bsp/esp-bsp.h"
#endif

#define TAG "HID Example"

struct RFIDHIDBridge {
    uint8_t packet_ID;
    uint8_t buffer[23];
};

// class RobotConnectPacket {
// public:
//     union {
//         uint8_t data[sizeof(PacketInfo)];
//         PacketInfo packet;
//     };
// };

uint8_t read_buffer[24];


char* lol2 = "firefox youtube.com/watch?v=dQw4w9WgXcQ&ab_channel=RickAstley";



void keystroke(uint8_t* key){
    uint8_t keycode[6] = {0};
    tinyusb_hid_keyboard_report(0, key);
    tinyusb_hid_keyboard_report(0, keycode);
}

void stringstroke(char* key_string, size_t length){
    for(size_t i=0; i<length; ++i){
    uint8_t const conv_table[128][2] =  { HID_ASCII_TO_KEYCODE };
    uint8_t keycode[6] = {0};
    uint8_t chr = (uint8_t)key_string[i];
    ESP_LOGI(TAG, "%d", (uint8_t)key_string[i]);
    if ( conv_table[chr][0] ){ //shift
        keycode[0] = HID_KEY_SHIFT_LEFT;
        keycode[1] = conv_table[chr][1];
        tinyusb_hid_keyboard_report(0, keycode);
        keycode[0] = 0;
        keycode[1] = 0;
    }
    else{
        keycode[0] = conv_table[chr][1];
        tinyusb_hid_keyboard_report(0, keycode);
        keycode[0] = 0;
        tinyusb_hid_keyboard_report(0, keycode);
    }
    //vTaskDelay(pdMS_TO_TICKS(50));
    }
}

static void tusb_device_task(void *arg) //DO not delete this.
{
    while (1) {
        tud_task();
    }
}

// Invoked when device is mounted
void tud_mount_cb(void)
{
    ESP_LOGD(TAG, "USB Mount");
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    ESP_LOGD(TAG, "USB Un-Mount");
}

void cb(const esp_now_recv_info_t *esp_now_info, const uint8_t *data, int len){
    // if(*data == RBT_DOCKED_VOLTAGE_STATUS){
    //     memcpy(&read_buffer, data, len);
    //     ESP_LOGI(TAG, "Received: %d", status.aux_voltage);
    //     }
    return;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    ESP_LOGD(TAG, "USB Suspend");
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    ESP_LOGD(TAG, "USB Resume");
}

//USB PHY config? idk what this is but it seems important
static void usb_phy_init(void)
{
    usb_phy_handle_t phy_hdl;
    // Configure USB PHY
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_OTG,
        .otg_mode = USB_OTG_MODE_DEVICE,
        .target = USB_PHY_TARGET_INT,
    };
    usb_new_phy(&phy_conf, &phy_hdl);
}

void app_main(void)
{
#ifdef CONFIG_ESP32_S3_USB_OTG
    bsp_usb_mode_select_device();
#endif
    // switch esp usb phy to usb-otg
    usb_phy_init();
    tud_init(BOARD_TUD_RHPORT);
    xTaskCreate(tusb_device_task, "TinyUSB", 4096, NULL, 5, NULL);
    tinyusb_hid_init();

    //initialize nvs
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //initialize wifi/espnow
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(14, WIFI_SECOND_CHAN_NONE));
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(cb));
    ESP_LOGI(TAG, "boot WIFI");
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1) {
        // uint8_t keycode[6] = {0};
        // keycode[0] = HID_KEY_ENTER;
        // tinyusb_hid_keyboard_report(0, keycode);
        uint8_t open_terminal[6] = {0};
        open_terminal[0] = HID_KEY_CONTROL_RIGHT;
        tinyusb_hid_keyboard_report(0, open_terminal);
        vTaskDelay(pdMS_TO_TICKS(100));
        open_terminal[1] = HID_KEY_ALT_RIGHT;
        tinyusb_hid_keyboard_report(0, open_terminal);
        vTaskDelay(pdMS_TO_TICKS(100));
        open_terminal[2] = HID_KEY_T;
        tinyusb_hid_keyboard_report(0, open_terminal);
        vTaskDelay(pdMS_TO_TICKS(100));
        open_terminal[0] = 0;
        open_terminal[1] = 0;
        open_terminal[2] = 0;
        vTaskDelay(pdMS_TO_TICKS(500));
        char* try = "aaaa";
        //keystroke(open_terminal);
                uint8_t keycode[6] = {0};

        stringstroke(try, 4);
        keycode[0] = HID_KEY_ENTER;
        tinyusb_hid_keyboard_report(0, keycode);
        keycode[0] = 0;
        tinyusb_hid_keyboard_report(0, keycode);
        vTaskDelay(pdMS_TO_TICKS(100));
        stringstroke(lol2, 62);
        keycode[0] = HID_KEY_ENTER;
        tinyusb_hid_keyboard_report(0, keycode);
        keycode[0] = 0;
        tinyusb_hid_keyboard_report(0, keycode);
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
