#include <stdio.h>
#include "include/i2c_utils.h"
#include "include/LM75A_driver.h"
#include "esp_log.h"
#include "esp_mac.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const static char *TAG = "MAIN";

/* void i2c_scan() {
    i2c_master_bus_handle_t bus;
    i2c_master_bus_config_t bus_config = {
        .i2c_port = MASTER_I2C_PORT,
        .sda_io_num = MASTER_SDA_PIN,
        .scl_io_num = MASTER_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus));

    ESP_LOGI(TAG, "Scanning I2C bus...");

    for (uint8_t addr = 0x03; addr < 0x77; addr++) {
        esp_err_t ret = i2c_master_probe(bus, addr, pdMS_TO_TICKS(100));
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Found device at 0x%02X", addr);
        }
    }

    ESP_LOGI(TAG, "Scan complete.");
}


void app_main(void) {
    i2c_scan();
} */

void app_main(void)
{   
    i2c_master_bus_handle_t bus_handle = NULL;
    LM75A_handle_t temp_1 = NULL;

    i2c_master_init(&bus_handle);
    
    for(int tries = 0; tries < 5; tries++) {
        if(bus_handle == NULL) {
            i2c_master_init(&bus_handle);
        }
        if(temp_1 == NULL) {
            temp_1 = LM75A_init(LM75A_UNIT_ADDRESS);
        }

        if(bus_handle != NULL && temp_1 != NULL) {
            ESP_LOGI(TAG, "Communication succesfully initiated");
            break;
        }
        else {
            ESP_LOGE(TAG, "Unable to initiate communication");
        }
    }
    
    while(1) {
        LM75A_i2c_read_temp(temp_1);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}