#ifndef LM75A_DRIVER_H
#define LM75A_DRIVER_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Set by connecting A0 - A2 to either VCC (1) or GND (0)
// LM75A uses a 7-bit adress whereas the 4 most significant bits are set as 1001.
#define LM75A_UNIT_ADDRESS            0x48
#define LM75A_UNIT_ADDRESS_LENGTH     7
#define LM75A_DATA_LENGTH             4           // Defines the size of data buffer

// The component four data registers and one additional Product ID register
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

typedef struct LM75A_handle_internal *LM75A_handle_t;

#endif