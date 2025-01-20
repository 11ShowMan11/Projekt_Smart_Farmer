idf_component_register(SRCS "Smart_Farmer_Firmware.c"
                    INCLUDE_DIRS "."
                    REQUIRES connection_component mqtt_component luminosity_component lightning_component humidity_component motors_component) 