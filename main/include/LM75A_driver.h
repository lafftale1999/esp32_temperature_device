#ifndef LM75A_DRIVER_H
#define LM75A_DRIVER_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/* ************************************************
                    SET UP
****************************************************/             

#define TEMPERATURE_UNIT 0            // Set to 1 for Fahrenheit or 0 for Celsius
#define READING_INTERVAL_MS 1000      // Set interval in MS for reading temperature 

#define LM75A_UNIT_ADDRESSES          {0x48}                // Array of addresses used on the I2C bus. Add your addresses here
#define LM75A_AMOUNT_OF_UNITS         1                     // How many units are connected to the I2C bus
#define LM75A_UNIT_ADDRESS_LENGTH     I2C_ADDR_BIT_LEN_7    // Address length - Either I2C_ADDR_BIT_LEN_7 or I2C_ADDR_BIT_LEN_10
#define LM75A_DATA_LENGTH             2                     // Defines the number of integers needed for the buffer when communicating

/* ************************************************
                    COMMANDS
****************************************************/ 

// The component has four data registers and one additional Product ID register
// Theses are selected by the Pointer Register
#define LM75A_TEMPERATURE_REGISTER    0x00        // READ ONLY
#define LM75A_CONFIGURATION_REGISTER  0x01        // READ / WRITE
#define LM75A_T_HYST_REGISTER         0x02        // READ / WRITE
#define LM75A_T_OS_REGISTER           0x03        // READ / WRITE
#define LM75A_PRODUCT_ID_REGISTER     0x07        // READ ONLY

// Configuration Register
// BIT 0 - 4 controls configurations
// BIT 5 - 7 should always be set to 0 - only used for production testing
#define LM75A_SHUTDOWN                (1 << 0)
#define LM75A_COMPARATOR_MODE         0
#define LM75A_INTERRUPT_MODE          (1 << 1)
#define LM75A_HIGH_POLARITY           (1 << 2)
#define LM75A_LOW_POLARITY            0

// Fault Queue in Configuration register
#define LM75A_FAULT_1_TIME            0           // Default
#define LM75A_FAULT_2_TIMES           0x08        
#define LM75A_FAULT_4_TIMES           0x10
#define LM75A_FAULT_6_TIMES           0x18

/* ************************************************
                    HANDLE
****************************************************/ 
typedef struct LM75A_handle_internal *LM75A_handle_t;

/* ************************************************
                    FUNCTIONS
****************************************************/ 

/*
    Reads the temperature from LM75A sensor and converts it
    to celsius. Saves the refined result in LM75A_handle_t->raw_temp

    @param LM75A_handle_t handle: A handle to access the relevant data of the device.

    @return esp_err_t ESP_ERR_INVALID_ARG: I2C master transmit parameter invalid.
    ESP_ERR_TIMEOUT: Operation timeout(larger than xfer_timeout_ms) because the bus is busy or hardware crash.
    ESP_FAIL: Unable to perform any of the operations for reading.
    ESP_OK: Temperature succesfully read and converted.
*/
void LM75A_i2c_read_temp_task(void *pvParameters);

/*
    Allocates the handles datastructure on the heap, fills it with the needed data and then redirects the passed
    pointer to the datastructure. Also creates and adds a device to the I2C bus.

    @param uint8_t device_address: The slave address for device used on the bus.
    @param LM75A_handle_t *out_handle: pointer to be set to heap allocated datastructure.

    @return esp_err_t 
    ESP_OK: Successfully initialized the LM75A.
    ESP_ERR_INVALID_ARG: Either I2C bus init failed because of invalid argument or unable to acquire bus handle.
    ESP_ERR_NO_MEM: Create I2C bus failed because of out of memory
    ESP_ERR_INVALID_STATE: Invalid state, such as the I2C port is not initialized.
    ESP_FAIL: Unable to allocate memory on heap for device handle or failed to create mutex.
*/
esp_err_t LM75A_init(LM75A_handle_t *out_handle, uint8_t device_address);

/*
    Deinitialize the LM75A datastructure.

    @param LM75A_handle_t handle: Pointer to datastructure to be removed.

    @return esp_err_t ESP_OK: Everything is A-OK.
*/
esp_err_t LM75A_deinit(LM75A_handle_t handle);

#endif