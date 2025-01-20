#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "i2c_component.h"
#include "mqtt_component.h"

void MotorsInitialize(void);
void X_AxisEnable(void);
void Y_AxisEnable(void);
void X_AxisDisable(void);
void Y_AxisDisable(void);
void X_AxisSetDirection(bool direciton);
void Y_AxisSetDirection(bool direction);
int X_AxisMove(int position, int current_position);
int Y_AxisMove(int position, int current_position);
void X_AxisZero(void);
void Y_AxisZero(void);
