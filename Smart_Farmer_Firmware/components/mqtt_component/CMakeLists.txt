idf_component_register (
    SRCS "mqtt_component.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event nvs_flash mqtt
)