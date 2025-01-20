#include "lightning_component.h"

static const int LUMINOSITY_HIST = 4;

uint8_t light[4] = {1, 2, 3, 4};
extern int luminosity[4];
extern adc_oneshot_unit_handle_t luminosity_handle;
extern esp_mqtt_client_handle_t client;
extern i2c_master_dev_handle_t device;

void Lightning()  {
    int luminosity_r = 0;
    for(int i = 0; i < 4; i++)  {
        switch(i) {
            case 0:
            luminosity_r = luminosity_read(luminosity_handle, LUMINOSITY_CHANNEL_1, client);
            break;

            case 1:
            luminosity_r = luminosity_read(luminosity_handle, LUMINOSITY_CHANNEL_2, client);
            break;

            case 2:
            luminosity_r = luminosity_read(luminosity_handle, LUMINOSITY_CHANNEL_3, client);
            break;

            case 3:
            luminosity_r = luminosity_read(luminosity_handle, LUMINOSITY_CHANNEL_4, client);
            break;
        }
        if((luminosity[i] - luminosity_r) > LUMINOSITY_HIST)  {
            i2c_setPin(light[i], true, device);
        }
        if((luminosity[i] - luminosity_r) < -1 * LUMINOSITY_HIST)  {
            i2c_setPin(light[i], false, device);
        }
    }
}