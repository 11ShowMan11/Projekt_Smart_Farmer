#include "luminosity_component.h"

static const char *TAG = "Luminosity component";
void luminosity_sensors_init(adc_oneshot_unit_handle_t *luminosity_handle)  {
    
    adc_oneshot_unit_init_cfg_t luminosity_unit_config = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&luminosity_unit_config, luminosity_handle));
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*luminosity_handle, LUMINOSITY_CHANNEL_1, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*luminosity_handle, LUMINOSITY_CHANNEL_2, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*luminosity_handle, LUMINOSITY_CHANNEL_3, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(*luminosity_handle, LUMINOSITY_CHANNEL_4, &config));

}

int luminosity_read(adc_oneshot_unit_handle_t luminosity_handle, int luminosity_channel, esp_mqtt_client_handle_t client)  {
        int result = 0;
        int raw_reading = 0;
        int percent = 0;
        for (int i = 0; i<10 ; i++)  {
            adc_oneshot_read(luminosity_handle, luminosity_channel, &raw_reading);
            result += raw_reading;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        result = result / 10; 
        ESP_LOGI(TAG, "Luminosity at channel %d: %d", luminosity_channel, result);
        percent = result * 100 / MAX_LUMINOSITY;  
        ESP_LOGI(TAG, "Luminosity percent at channel %d: %d",luminosity_channel, percent);

        if (percent > 100)  {
            percent = 100;
        }
        char percent_mqtt[] = {
        (percent / 100) + '0',
        (percent % 100 / 10) + '0',
        (percent % 10) + '0',
        '\0'
        };
        switch(luminosity_channel)  {
        case LUMINOSITY_CHANNEL_1:
        esp_mqtt_client_publish(client, LUMINOSITY_R_TOPIC_1, percent_mqtt, 0, 0, 0);
        break;

        case LUMINOSITY_CHANNEL_2:
        esp_mqtt_client_publish(client, LUMINOSITY_R_TOPIC_2, percent_mqtt, 0, 0, 0);
        break;

        case LUMINOSITY_CHANNEL_3:
        esp_mqtt_client_publish(client, LUMINOSITY_R_TOPIC_3, percent_mqtt, 0, 0, 0);
        break;

        case LUMINOSITY_CHANNEL_4:
        esp_mqtt_client_publish(client, LUMINOSITY_R_TOPIC_4, percent_mqtt, 0, 0, 0);
        break;

        default:
        break;
        }
        return percent;
}