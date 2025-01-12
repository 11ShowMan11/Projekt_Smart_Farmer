#include <stdio.h>
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"

#define I2C_SCL_IO 22
#define I2C_SDA_IO 21

#define PIN_DIR_A 0x00
#define PIN_DIR_B 0x01
#define PIN_SET_A 0x12
#define PIN_SET_B 0x13
#define PIN_STATUS_A 0x14
#define PIN_STATUS_B 0x15

void bitWrite(uint8_t *value, uint8_t bit, uint8_t bitValue);
void i2c_init(i2c_master_dev_handle_t *device_handle);
void i2c_setPin(uint8_t pin, bool state, i2c_master_dev_handle_t device_handle);
bool i2c_readPin(uint8_t pin, i2c_master_dev_handle_t device_handle);