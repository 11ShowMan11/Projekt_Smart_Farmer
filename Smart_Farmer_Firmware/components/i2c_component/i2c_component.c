#include "i2c_component.h"


void bitWrite(uint8_t *value, uint8_t bit, uint8_t bitValue) {
    if (bitValue) {
        *value |= (1 << bit); 
    } else {
        *value &= ~(1 << bit);
    }
}

void i2c_init(i2c_master_dev_handle_t *device_handle)  {
    i2c_master_bus_config_t i2c_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = -1,
        .scl_io_num = I2C_SCL_IO,
        .sda_io_num = I2C_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    i2c_master_bus_handle_t i2c_handle = NULL;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_handle));

    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = 0x27,
        .scl_speed_hz = 100000,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_handle, &device_config, device_handle));

    uint8_t pins_directions_A[] = {PIN_DIR_A, 0xC0}; // 1100 0000 - 6 outputs 2 inputs
    ESP_ERROR_CHECK(i2c_master_transmit(*device_handle,
                                        pins_directions_A,
                                        sizeof(pins_directions_A),
                                        -1));

    uint8_t pins_directions_B[] = {PIN_DIR_B, 0xF0}; // 1111 0000 - 4 inputs and 4 outputs
    ESP_ERROR_CHECK(i2c_master_transmit(*device_handle,
                                       pins_directions_B,
                                       sizeof(pins_directions_B),
                                       -1));
}

void i2c_setPin(uint8_t pin, bool state, i2c_master_dev_handle_t device_handle)  {
    uint8_t adr = 0x00;
    uint8_t current_state = 0x00;
    if(pin > 8)  {
        adr = PIN_STATUS_B;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle,
                                                    
                                                    &adr,
                                                    sizeof(adr),
                                                    &current_state,
                                                    sizeof(current_state),
                                                    -1));
        bitWrite(&current_state, pin - 9, state);
        adr = PIN_SET_B;
    }
    else  {
        adr = PIN_STATUS_A;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle,
                                                    &adr,
                                                    sizeof(adr),
                                                    &current_state,
                                                    sizeof(current_state),
                                                    -1));
        bitWrite(&current_state, pin - 1, state);
        adr = PIN_SET_A;
    }


    uint8_t data[] = {adr, current_state};
    ESP_ERROR_CHECK(i2c_master_transmit(device_handle, data, sizeof(data), -1));
}

bool i2c_readPin(uint8_t pin, i2c_master_dev_handle_t device_handle)  {
    uint8_t adr = 0x00;
    uint8_t current_state = 0x00;
    uint8_t value = 0x00;
    if(pin > 8)  {
        adr = PIN_SET_B;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle,
                                                    
                                                    &adr,
                                                    sizeof(adr),
                                                    &current_state,
                                                    sizeof(current_state),
                                                    -1));
        bitWrite(&value, pin - 9, 1);
        value = value & current_state;

        if(value)  {
            return true;
        }
        else  {
            return false;
        }
    }
    else  {
        adr = PIN_SET_A;
        ESP_ERROR_CHECK(i2c_master_transmit_receive(device_handle,
                                                    &adr,
                                                    sizeof(adr),
                                                    &current_state,
                                                    sizeof(current_state),
                                                    -1));
        bitWrite(&value, pin - 1, 1);
        value = value & current_state;
        if(value)  {
            return true;
        }
        else  {
            return false;
        }
    }
}
