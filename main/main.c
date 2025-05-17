#include <stdio.h>
#include "include/i2c_utils.h"
#include "include/LM75A_driver.h"
#include "esp_log.h"


void app_main(void)
{
    uint8_t data[2];
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t device_handle;

    i2c_master_init(&bus_handle, &device_handle);
    
    
}