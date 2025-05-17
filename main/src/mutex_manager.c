

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <esp_log.h>
#include <esp_mac.h>

const char *TAG = "Semaphore Manager";

uint8_t mutex_manager_init(void) {
    if (mother_mutex == NULL) {
        mother_mutex = xSemaphoreCreateMutex();
        ESP_LOGI(TAG, "Mutex succesfully created...");
        return 0;
    }

    else {
        ESP_LOGE(TAG, "Mutex already created...");
        return 1;
    }
}

SemaphoreHandle_t get_mother_mutex(void) {
    if(mother_mutex == NULL) {
        ESP_LOGE(TAG, "Mutex hasn't been created. Use mutex_manager_init() before calling get_mother_mutex()");
    }
    return mother_mutex;
}