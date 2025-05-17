#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

#define MASTER_SDA_PIN          GPIO_NUM_7      // GPIO for SDA
#define MASTER_SCL_PIN          GPIO_NUM_6      // GPIO for SCL
#define MASTER_I2C_PORT         I2C_NUM_0       // ESP32 Internal I2C module
#define MASTER_FREQUENCY        100000          // Unit: Hz
#define MASTER_TX_BUF_DISABLE   0
#define MASTER_RX_BUF_DISABLE   0
#define MASTER_TIMEOUT_MS       1000

#define MAX_AMOUNT_OF_TRIES     3
#define WAIT_FOR_MUTEX_MS       50

esp_err_t i2c_register_read(i2c_master_dev_handle_t device_handle, uint8_t register_address, uint8_t *data, size_t len);
esp_err_t i2c_register_write_byte(i2c_master_dev_handle_t device_handle, uint8_t register_address, uint8_t data);
void i2c_master_init(i2c_master_bus_handle_t *bus_handle);

#endif