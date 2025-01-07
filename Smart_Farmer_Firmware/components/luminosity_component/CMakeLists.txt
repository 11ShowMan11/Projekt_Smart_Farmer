idf_component_register (
    SRCS "luminosity_component.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_adc mqtt_component
)