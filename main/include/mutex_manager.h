#ifndef MUTEX_MANAGER_H
#define MUTEX_MANAGER_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

uint8_t mutex_manager_init(void);
SemaphoreHandle_t get_mother_mutex(void);

#endif