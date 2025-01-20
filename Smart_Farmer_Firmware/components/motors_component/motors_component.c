#include "motors_component.h"

static const char *TAG = "Motors control";

extern i2c_master_dev_handle_t device;

// X Axis pins
#define X_AXIS_ENABLE_PIN      10
#define X_AXIS_DIRECTION_PIN   9
#define X_AXIS_PWM_PIN         19

// Y Axis pins
#define Y_AXIS_ENABLE_PIN      12
#define Y_AXIS_DIRECTION_PIN   11
#define Y_AXIS_PWM_PIN         18

#define PWM_FREQUENCY   5000  // Hz
#define PWM_RESOLUTION  LEDC_TIMER_8_BIT
#define PWM_CHANNEL     LEDC_CHANNEL_0

// X Axis physical parameters
bool motor_busy =       false;
#define RIGHT           true
#define LEFT            false
#define X_AXIS_DIAMETER 2
#define X_AXIS_LENGHT   50
#define X_AXIS_ENDSTOP  13


#define Y_AXIS_DIAMETER 2
#define Y_AXIS_LENGHT   50
#define Y_AXIS_ENDSTOP  14
// X Axis signals
static bool x_axis_endstop = false;
static bool y_axis_endstop = false;
extern bool reset;

// MQTT client handle for comunication
extern esp_mqtt_client_handle_t client_handle;

void MotorsInitialize(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = LEDC_TIMER_0,
        .duty_resolution  = PWM_RESOLUTION,
        .freq_hz          = PWM_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t x_axis_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = PWM_CHANNEL,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = X_AXIS_PWM_PIN,
        .duty           = 127, 
        .hpoint         = 0
    };

    ledc_channel_config_t y_axis_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = PWM_CHANNEL,
        .timer_sel      = LEDC_TIMER_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = Y_AXIS_PWM_PIN,
        .duty           = 127, 
        .hpoint         = 0
    };
    ledc_channel_config(&x_axis_channel);
    ledc_channel_config(&y_axis_channel);
    ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
    ESP_LOGI(TAG, "Motors initialized.");
}

void X_AxisEnable(void) {
    i2c_setPin(X_AXIS_ENABLE_PIN, false, device);
}

void Y_AxisEnable(void) {
    i2c_setPin(Y_AXIS_ENABLE_PIN, false, device);
}

void X_AxisDisable(void) {
    i2c_setPin(X_AXIS_ENABLE_PIN, true, device);
}

void Y_AxisDisable(void) {
    i2c_setPin(Y_AXIS_ENABLE_PIN, true, device);
}

void X_AxisSetDirection(bool direction) {
    i2c_setPin(X_AXIS_DIRECTION_PIN, direction ? 1 : 0, device);
}

void Y_AxisSetDirection(bool direction) {
    i2c_setPin(Y_AXIS_DIRECTION_PIN, direction ? 1 : 0, device);
}

int X_AxisMove(int position, int current_position) {
    ESP_LOGI(TAG, "X Axis moving to: %i", position);
    if (position > current_position)  {
        X_AxisSetDirection(RIGHT);
    }
    else if(position < current_position) {
        X_AxisSetDirection(LEFT);
    }
    X_AxisEnable();
    ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
    vTaskDelay(pdMS_TO_TICKS(abs(current_position - position) * 84.93 / X_AXIS_DIAMETER));
    ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
    X_AxisDisable();
    return position;
}

int Y_AxisMove(int position, int current_position) {
    ESP_LOGI(TAG, "Y Axis moving to: %i", position);
    motor_busy = true;
    if (position > current_position)  {
        Y_AxisSetDirection(RIGHT);
    }
    else  {
        Y_AxisSetDirection(LEFT);
    }
    Y_AxisEnable();
    ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
    vTaskDelay(pdMS_TO_TICKS(abs(current_position - position) * 84.93 / Y_AXIS_DIAMETER));
    ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
    Y_AxisDisable();
    motor_busy = false;
    return position;
}

void X_AxisZero(void)  {
    uint8_t state = 1;
    switch (state)  {
        case 1: 
        motor_busy = true;
        X_AxisSetDirection(LEFT);
        X_AxisEnable();
        ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
        while(!x_axis_endstop)  {
            x_axis_endstop = i2c_readPin(X_AXIS_ENDSTOP, device);
            vTaskDelay(pdMS_TO_TICKS(100));
            int64_t start_time = esp_timer_get_time();
            int64_t elapsed_time = esp_timer_get_time() - start_time;
            if(elapsed_time > (X_AXIS_LENGHT + 10) * 84.93 / X_AXIS_DIAMETER)  {
                //send mqtt message
                esp_mqtt_client_publish(client_handle, ERROR_TOPIC, "1", 2, 0, 0);
                ESP_LOGI(TAG, "X Axis cannot find zero.");
                state = 2;
                break;
            }
        }
        ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
        X_AxisDisable();
        if(x_axis_endstop)  {
            state = 3;
        }

        case 2:
        while(!reset)  {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        state = 1;
        break;

        case 3:
        ESP_LOGI(TAG, "X Axis found zero.");
        break;
    }
}

void Y_AxisZero(void)  {
    uint8_t state = 1;
    switch (state)  {
        case 1: 
        motor_busy = true;
        Y_AxisSetDirection(RIGHT);
        Y_AxisEnable();
        ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
        while(!y_axis_endstop)  {
            y_axis_endstop = i2c_readPin(Y_AXIS_ENDSTOP, device);
            vTaskDelay(pdMS_TO_TICKS(100));
            int64_t start_time = esp_timer_get_time();
            int64_t elapsed_time = esp_timer_get_time() - start_time;
            if(elapsed_time > (Y_AXIS_LENGHT + 10) * 84.93 / Y_AXIS_DIAMETER)  {
                //send mqtt message
                esp_mqtt_client_publish(client_handle, ERROR_TOPIC, "2", 2, 0, 0);
                ESP_LOGI(TAG, "Y Axis cannot find zero.");
                state = 2;
                break;
            }
        }
        ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);
        Y_AxisDisable();
        if(y_axis_endstop)  {
            state = 3;
        }

        case 2:
        while(!reset)  {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        state = 1;
        break;

        case 3:
        ESP_LOGI(TAG, "Y Axis found zero.");
        break;
    }
}