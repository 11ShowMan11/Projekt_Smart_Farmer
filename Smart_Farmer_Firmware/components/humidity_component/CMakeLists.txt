idf_component_register (
    SRCS "humidity_component.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_adc mqtt_component
)