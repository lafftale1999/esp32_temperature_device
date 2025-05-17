#include "include/i2c_utils.h"
#include "include/LM75A_driver.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_log.h>
#include <esp_mac.h>

const static char *TAG = "I2C_MASTER";

static SemaphoreHandle_t i2c_mutex;

esp_err_t i2c_register_read(i2c_master_dev_handle_t device_handle, uint8_t register_address, uint8_t *data, size_t len) {
    if(i2c_mutex == NULL) {
        ESP_LOGE(TAG, "Mutex not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if(device_handle == NULL) {
        ESP_LOGE(TAG, "Device handle is NULL before I2C read");
    }
    
    esp_err_t ret = ESP_FAIL;
    
    for(uint8_t tries = 0; tries < MAX_AMOUNT_OF_TRIES; tries++) {
        if(xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(WAIT_FOR_MUTEX_MS)) == pdTRUE) {
            ret = i2c_master_transmit_receive(device_handle, &register_address, 1, data, len, MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
            xSemaphoreGive(i2c_mutex);
            break;
        }
    }
    
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read register: %s", esp_err_to_name(ret));
    }

    return ret;
}

esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t device_handle, uint8_t register_address, uint8_t data) {
    if(i2c_mutex == NULL) {
        ESP_LOGE(TAG, "Mutex not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    
    esp_err_t ret = ESP_FAIL;

    uint8_t buffer[2] = {register_address, data};

    for(uint8_t tries = 0; tries < MAX_AMOUNT_OF_TRIES; tries++) {
        if(xSemaphoreTake(i2c_mutex, pdMS_TO_TICKS(WAIT_FOR_MUTEX_MS)) == pdTRUE) {
            ret = i2c_master_transmit(device_handle, buffer, sizeof(buffer), MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
            xSemaphoreGive(i2c_mutex);
            break;
        }
    }

    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write to device");
    }

    return ret; 
}

void i2c_master_init(i2c_master_bus_handle_t *bus_handle) {
    i2c_master_bus_config_t bus_config = {
        .i2c_port = MASTER_I2C_PORT,
        .sda_io_num = MASTER_SDA_PIN,
        .scl_io_num = MASTER_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));

    i2c_mutex = xSemaphoreCreateMutex();

    if(i2c_mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create mutex...");
        return;
    }

    ESP_LOGI(TAG, "I2C successfully initialized");
}