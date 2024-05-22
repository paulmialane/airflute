#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define EXT_GPIO_CTRL_1 DT_NODELABEL(pcf8574_1)
//structure pour contrôler le device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

int getBit(int n, int k)
{
    return (n & (1<<k)) != 0;
}

gpio_port_value_t val_gpio_1;


//fonction pour le traitement
int *is_pressed(void){
    gpio_port_get(ext_gpio_1, &val_gpio_1);

    return (int*) &val_gpio_1;
}
