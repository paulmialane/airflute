#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define EXT_GPIO_CTRL_1 DT_NODELABEL(pcf8574_1)
// structure to control the device
static const struct device * const ext_gpio_1 = DEVICE_DT_GET(EXT_GPIO_CTRL_1);

#define SLEEP_TIME_MS 1000

gpio_port_value_t val_gpio_1;


// A function to check if a button is pressed
int getKeyMask(void){
    gpio_port_get(ext_gpio_1, &val_gpio_1);
    // We invert the value because the button is active low
    return 0xff - val_gpio_1;
}
