#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "esp_event.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_mac.h"

void wifi_handler( void *,
                          esp_event_base_t ,
                         int32_t , 
                         void *);   
 void WiFiConnect(char*, char*);
 void WiFiCreate();
