idf_component_register (
    SRCS "lightning_component.c"
    INCLUDE_DIRS "include"
    REQUIRES i2c_component luminosity_component
)