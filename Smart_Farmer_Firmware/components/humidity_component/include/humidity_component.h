#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "mqtt_component.h"

#define HUMIDITY_CHANNEL_1          ADC_CHANNEL_8
#define HUMIDITY_CHANNEL_2          ADC_CHANNEL_9
#define HUMIDITY_CHANNEL_3          ADC_CHANNEL_7
#define HUMIDITY_CHANNEL_4          ADC_CHANNEL_6


#define HUMIDITY_R_TOPIC_1      "d1/wr\0"
#define HUMIDITY_R_TOPIC_2      "d2/wr\0"
#define HUMIDITY_R_TOPIC_3      "d3/wr\0"
#define HUMIDITY_R_TOPIC_4      "d4/wr\0"

#define EXAMPLE_ADC_ATTEN             ADC_ATTEN_DB_12
#define MAX_HUMIDITY                2600

void humiditySensorsInit(adc_oneshot_unit_handle_t *humidity_handle);

int humidityRead(adc_oneshot_unit_handle_t humidity_handle, int humidity_channel, esp_mqtt_client_handle_t client);
