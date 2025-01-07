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

#define LUMINOSITY_CHANNEL_1          ADC_CHANNEL_0
#define LUMINOSITY_CHANNEL_2          ADC_CHANNEL_3
#define LUMINOSITY_CHANNEL_3          ADC_CHANNEL_6
#define LUMINOSITY_CHANNEL_4          ADC_CHANNEL_7


#define LUMINOSITY_R_TOPIC_1      "d1/nr\0"
#define LUMINOSITY_R_TOPIC_2      "d2/nr\0"
#define LUMINOSITY_R_TOPIC_3      "d3/nr\0"
#define LUMINOSITY_R_TOPIC_4      "d4/nr\0"

#define EXAMPLE_ADC_ATTEN             ADC_ATTEN_DB_12
#define MAX_LUMINOSITY                2600

void luminosity_sensors_init(adc_oneshot_unit_handle_t *luminosity_handle);

int luminosity_read(adc_oneshot_unit_handle_t luminosity_handle, int luminosity_channel, esp_mqtt_client_handle_t client);
