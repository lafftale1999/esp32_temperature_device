#include "include/LM75A_driver.h"
#include "include/i2c_utils.h"

#include <esp_log.h>
#include <esp_mac.h>

const static char *TAG = "LM75A";

struct LM75A_handle_internal{
    uint8_t address;
    uint8_t address_length;
    uint8_t data[LM75A_DATA_LENGTH];
    uint8_t data_len;
    float raw_temp;
    i2c_master_dev_handle_t device;
    SemaphoreHandle_t xMutex;
};

void LM75A_i2c_read_temp_task(void *pvParameters) {
    LM75A_handle_t handle = (LM75A_handle_t) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(1) {
        if(handle == NULL) {
        ESP_LOGE(TAG, "Unable to parse handle when reading temperature");
        }

        else {
            esp_err_t err = ESP_FAIL;
            if(xSemaphoreTake(handle->xMutex, pdMS_TO_TICKS(WAIT_FOR_MUTEX_MS)) == pdTRUE) {
                err = i2c_register_read(handle->device, LM75A_TEMPERATURE_REGISTER, handle->data, handle->data_len);
                
                /*
                    When reading temp - LM75BD returns:
                    data[0] = MSB (8 bits)
                    data[1] = LSB (8 bits)

                    The result is a signed 11-bit value in two's complement form,
                    whereas only the most significant 11-bits are of value for temperature readings.

                    Down below is NXP recommended way to convert the information to Celsius.
                */
                int16_t raw = ((int16_t)handle->data[0] << 8) | handle->data[1];
                raw >>= 5;
                handle->raw_temp = (float)(raw * 0.125f);

                if(TEMPERATURE_UNIT){
                    handle->raw_temp = (handle->raw_temp * 9.0f / 5.0f) + 32.0f;
                }
                // Temp converted
                
                if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to read temperature: %s", esp_err_to_name(err));
                } else {
                    ESP_LOGI(TAG, "LM75A Temp: %.1f %s", handle->raw_temp, TEMPERATURE_UNIT ? "F" : "C");
                }

                xSemaphoreGive(handle->xMutex);
            } else {
                ESP_LOGE(TAG, "Failed to acquire mutex: %s", esp_err_to_name(err));
            }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

SemaphoreHandle_t get_LM75a_mutex(LM75A_handle_t handle) {
    if(handle->xMutex != NULL) {
        return handle->xMutex;
    }
    else {
        ESP_LOGE(TAG, "Mutex not created");
        return NULL;
    }
    
}

esp_err_t LM75A_init(LM75A_handle_t *out_handle, uint8_t device_address) {
    LM75A_handle_t handle = malloc(sizeof(struct LM75A_handle_internal));
    esp_err_t err = ESP_FAIL;

    if(handle == NULL) {
        ESP_LOGE(TAG, "Unable to allocate memory for device handle...");
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

    i2c_master_bus_handle_t bus;
    err = i2c_master_get_bus_handle(MASTER_I2C_PORT, &bus);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Unable to acquire bus handle");
        goto exit;
    }

    err = i2c_master_bus_add_device(bus, &dev_config, &handle->device);

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device to buss...");
        goto exit;
    }

    ESP_LOGI(TAG, "LM75A initialized on address: %d", handle->address);
    *out_handle = handle;
    return ESP_OK;

exit:
    if(handle != NULL) {
        if(handle->xMutex != NULL) {
            vSemaphoreDelete(handle->xMutex);
        }

        if(handle->device) {
            i2c_master_bus_rm_device(handle->device);
        }

        free(handle);
    }
    
    *out_handle = NULL;
    return err;
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