#include "connection_component.h"
#include "connection_credentials.h"

static const char* CON_TAG = "CONNECTION" ;

void wifi_handler( void *event_handler_arg, 
                                esp_event_base_t event_base,
                                int32_t event_id, 
                                void *event_data)    {

    if(event_id == WIFI_EVENT_STA_START)    {
        ESP_LOGI(CON_TAG, "WiFi trying to connect.");
    }

    else if (event_id == WIFI_EVENT_STA_CONNECTED)  {
        ESP_LOGI(CON_TAG, "WiFi connected.");
    }

    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)   {
        ESP_LOGI(CON_TAG, "WiFi lost connection.");
        esp_wifi_connect();
    }

    else if (event_id == IP_EVENT_STA_GOT_IP)    {
        ESP_LOGI(CON_TAG, "Farmer got IP.");
    }
}

void WiFiInitialize()   {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initial_configuration = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initial_configuration);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_handler, NULL);
    esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, wifi_handler, NULL);

}

void WiFiConnect()    {

    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = "",
            .password = ""
        }
    };

    strcpy((char*)wifi_configuration.sta.ssid, SSID);
    strcpy((char*)wifi_configuration.sta.password, PASSWORD);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();
    vTaskDelay(pdMS_TO_TICKS(1000));
}