idf_component_register (
    SRCS "motors_component.c"
    INCLUDE_DIRS "include"
    REQUIRES i2c_component esp_timer mqtt_component
)