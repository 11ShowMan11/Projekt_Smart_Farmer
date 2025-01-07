#include "mqtt_component.h"


extern esp_mqtt_client_handle_t client;
const static char* TAG = "MQTT";

 void mqttEventHandler( void *handler_args,
                                esp_event_base_t base,
                                int32_t event_id, 
                                void *event_data)  {

    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Broker connected.");
        
        msg_id = esp_mqtt_client_subscribe(client, "device/#", 0);
        ESP_LOGI(TAG, "Subscribed successful, msg_id=%d", msg_id);

        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Broker lost connection.");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "Subscribed, message id: %d", event->msg_id);
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "Unsubscribed, message id: %d",event->msg_id);
        break;

    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "Published, message id: %d", event->msg_id);
        break;
        
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Message recived:");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "Error not handled.");
        break;
        
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}


esp_mqtt_client_handle_t mqttGetClient(void)  {
return client;
}

void mqttInitialize(char* uri)  {
    const esp_mqtt_client_config_t mqtt_cfg = {
       .broker.address.uri = uri,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqttEventHandler, NULL); 
    esp_mqtt_client_start(client);
} 

void mqttCreateURI(char* uri, char* MQTT_USR, char* MQTT_PASS, char* MQTT_IP)  {
    strcpy(uri, "mqtt://");
    strcat(uri, MQTT_USR);
    strcat(uri, ":");
    strcat(uri, MQTT_PASS);
    strcat(uri, "@");
    strcat(uri, MQTT_IP); 
    strcat(uri, ":1883");
}