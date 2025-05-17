#include "include/LM75A_driver.h"
#include "include/i2c_utils.h"

#include <esp_log.h>
#include <esp_mac.h>

const char *TAG = "LM75A";

struct LM75A_handle_internal{
    uint8_t address;
    uint8_t address_length;
    uint8_t data[LM75A_DATA_LENGTH];
    uint8_t data_len;
    float raw_temp
    i2c_master_dev_handle_t device;
    SemaphoreHandle_t xMutex;
};

esp_err_t LM75A_i2c_read_temp(LM75A_handle_t handle) {
    esp_err_t ret = ESP_FAIL;
    if(xSemaphoreTake(handle->xMutex, pdMS_TO_TICKS(WAIT_FOR_MUTEX_MS)) == pdTRUE) {
        ret = i2c_register_read(handle->device, LM75A_TEMPERATURE_REGISTER, handle->data, handle->data_len);
        xSemaphoreGive(handle->xMutex);
    }

    return ret;
}

LM75A_handle_t LM75A_init(uint8_t device_address) {
    LM75A_handle_t handle = malloc(sizeof(struct LM75A_handle_internal));

    if(handle == NULL) {
        ESP_LOGE(TAG, "Unable to create device handle...");
        goto exit;
    }

    handle->address = device_address;
    handle->address_length = LM75A_UNIT_ADDRESS_LENGTH;
    handle->data_len = LM75A_DATA_LENGTH;
    handle->xMutex = xSemaphoreCreateMutex();

    if(handle->xMutex == NULL) {
        ESP_LOGE(TAG, "Failed to create mutex...");
        goto exit;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = LM75A_UNIT_ADDRESS_LENGTH,
        .device_address = device_address,
        .scl_speed_hz = MASTER_FREQUENCY
    };

    i2c_master_bus_handle_t bus = i2c_master_get_bus_handle();
    if(bus == NULL) {
        ESP_LOGE(TAG, "Unable to acquire bus handle");
        goto exit;
    }

    if(i2c_master_bus_add_device(bus, &dev_config, &handle->device) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device to buss...");
        goto exit;
    }

    ESP_LOGI(TAG, "LM75A initialized");
    return handle;

    exit:
    if(handle != NULL) {
        if(handle->xMutex != NULL) {
            vSemaphoreDelete(handle->xMutex);
        }
        free(handle);
    }
    
    return NULL;
}

esp_err_t LM75A_deinit(LM75A_handle_t handle) {
    if(handle == NULL) return ESP_OK;

    if(handle->device != NULL) {
        i2c_master_bus_rm_device(handle->device);
    }

    if(handle->xMutex != NULL) {
        vSemaphoreDelete(handle->xMutex);
    }

    free(handle);

    return ESP_OK;
}