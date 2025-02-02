#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "mqtt_client.h"

#define ERROR_TOPIC "b"

void mqttEventHandler(void* ,
                             esp_event_base_t ,
                             int32_t , 
                             void* ); 

esp_mqtt_client_handle_t mqttGetClient(void);  

void mqttInitialize(); 

void mqttCreateURI(char*, char*, char*, char*);  

void mqttMessageHandler(int topic_len, char* topic, int data_len, char* data);