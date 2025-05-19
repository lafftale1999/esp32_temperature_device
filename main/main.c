#include <stdio.h>
#include "include/i2c_utils.h"
#include "include/wifi_utils.h"
#include "include/LM75A_driver.h"
#include "esp_log.h"
#include "esp_mac.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

const static char *TAG = "MAIN";

void app_main(void)
{   
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    i2c_master_bus_handle_t bus_handle = NULL;
    LM75A_handle_t devices[LM75A_AMOUNT_OF_UNITS];
    uint8_t device_addresses[] = LM75A_UNIT_ADDRESSES;

    ESP_ERROR_CHECK(wifi_init());
    ESP_ERROR_CHECK(i2c_master_init(&bus_handle));

    for(int i = 0; i < LM75A_AMOUNT_OF_UNITS; i++) {
        ESP_ERROR_CHECK(LM75A_init(&devices[i], device_addresses[i]));
        char task_name[16];
        snprintf(task_name, sizeof(task_name), "temp_task_%d", i);
        xTaskCreate(LM75A_i2c_read_temp_task, task_name, 2048, devices[i], 5, NULL);
    }
}