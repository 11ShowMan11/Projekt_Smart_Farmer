#include "mqtt_component.h"
#include "mqtt_credentials.h"


extern esp_mqtt_client_handle_t client;

extern int humidity[4];      // Global table for reading humidity
extern int luminosity[4];    // Global table for reading luminosity
extern int sow[4];           // Global table for reading sowing 
extern bool reset;           // Global variable for reading reset

const static char* TAG = "MQTT"; // Tag for recognising message source

/**
 * @brief Function generates topic from a template
 * 
 * The function generates something like this: "d(number)/(topic)"
 * 
 * @param buf String will be generated in this location must be 5 char's long
 * @param topic letter should be: "w" - humidity, "n" - luminosity, "s" - sowing
 * @param number number of desired plant 1 to 4
 *
 */
void generateTopic(char *buf, char topic, int number)  {
    sniprintf(buf, 5, "d%d/%c", number, topic);
}

/**
 * @brief Function for handling mqtt events
 */
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
        
        msg_id = esp_mqtt_client_subscribe(client, "d1/#", 0);
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
        mqttMessageHandler(event->topic_len,
                           event->topic,
                           event->data_len,
                           event->data);
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "Error not handled.");
        break;
        
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

/**
 * @brief Function starts mqtt comunication
 */
void mqttInitialize()  {
    char uri[60];
    mqttCreateURI(uri, MQTT_USERNAME, MQTT_PASSWORD, IP);
    const esp_mqtt_client_config_t mqtt_cfg = {
       .broker.address.uri = uri,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqttEventHandler, NULL); 
    esp_mqtt_client_start(client);
    vTaskDelay(pdMS_TO_TICKS(1000));
} 

/**
 * @brief Helper function to create connection uri with given parameters
 */
void mqttCreateURI(char* uri, char* MQTT_USR, char* MQTT_PASS, char* MQTT_IP)  {
    strcpy(uri, "mqtt://");
    strcat(uri, MQTT_USR);
    strcat(uri, ":");
    strcat(uri, MQTT_PASS);
    strcat(uri, "@");
    strcat(uri, MQTT_IP); 
    strcat(uri, ":1883");
}

/**
 * @brief Decoding messages and distributing it to global variables
 */
void mqttMessageHandler(int topic_len, char* topic, int data_len, char* data)  {
    char topic_b[5];
    char topic_cmp[5];
    char data_b[4];

    strncpy(data_b, data, data_len);
    data_b[4] = '\0';
    strncpy(topic_b, topic, topic_len);
    topic_b[4] = '\0';

    for(int i = 1; i <= 4; i++)  {
        generateTopic(topic_cmp, 'w', i);
        if(!strcmp(topic_b, topic_cmp))  {
            humidity[i] = atoi(data_b);
        }
        generateTopic(topic_cmp, 'n', i);
        if(!strcmp(topic_b, topic_cmp))  {
            luminosity[i] = atoi(data_b);
        }
        generateTopic(topic_cmp, 's', i);
        if(!strcmp(topic_b, topic_cmp))  {
            sow[i] = atoi(data_b);
        }
    }
    if(!strcmp(topic_b, "/r"))  {
        reset = data_b[1] - '0';
    }
}